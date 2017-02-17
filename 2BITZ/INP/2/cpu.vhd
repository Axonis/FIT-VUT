-- cpu.vhd: Simple 8-bit CPU (BrainLove interpreter)
-- Copyright (C) 2016 Brno University of Technology,
-- Faculty of Information Technology
-- Author(s): xurban66
--

LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
USE ieee.std_logic_arith.ALL;
USE ieee.std_logic_unsigned.ALL;

-- ----------------------------------------------------------------------------
-- Entity declaration
-- ----------------------------------------------------------------------------
ENTITY cpu IS
	PORT (
		CLK   : IN std_logic; -- hodinovy signal
		RESET : IN std_logic; -- asynchronni reset procesoru
		EN    : IN std_logic; -- povoleni cinnosti procesoru

		-- synchronni pamet ROM
		CODE_ADDR : OUT std_logic_vector(11 DOWNTO 0); -- adresa do pameti
		CODE_DATA : IN std_logic_vector(7 DOWNTO 0); -- CODE_DATA <- rom[CODE_ADDR] pokud CODE_EN='1'
		CODE_EN   : OUT std_logic; -- povoleni cinnosti

		-- synchronni pamet RAM
		DATA_ADDR  : OUT std_logic_vector(9 DOWNTO 0); -- adresa do pameti
		DATA_WDATA : OUT std_logic_vector(7 DOWNTO 0); -- mem[DATA_ADDR] <- DATA_WDATA pokud DATA_EN='1'
		DATA_RDATA : IN std_logic_vector(7 DOWNTO 0); -- DATA_RDATA <- ram[DATA_ADDR] pokud DATA_EN='1'
		DATA_RDWR  : OUT std_logic; -- cteni (1) / zapis (0)
		DATA_EN    : OUT std_logic; -- povoleni cinnosti

		-- vstupni port
		IN_DATA : IN std_logic_vector(7 DOWNTO 0); -- IN_DATA <- stav klavesnice pokud IN_VLD='1' a IN_REQ='1'
		IN_VLD  : IN std_logic; -- data platna
		IN_REQ  : OUT std_logic; -- pozadavek na vstup data

		-- vystupni port
		OUT_DATA : OUT std_logic_vector(7 DOWNTO 0); -- zapisovana data
		OUT_BUSY : IN std_logic; -- LCD je zaneprazdnen (1), nelze zapisovat
		OUT_WE   : OUT std_logic -- LCD <- OUT_DATA pokud OUT_WE='1' a OUT_BUSY='0'
	);
END cpu;
-- ----------------------------------------------------------------------------
-- Architecture declaration
-- ----------------------------------------------------------------------------
ARCHITECTURE behavioral OF cpu IS
 
	SIGNAL pc_inc         : std_logic;
	SIGNAL pc_dec         : std_logic;
	SIGNAL pc_data        : std_logic_vector(11 DOWNTO 0);

	SIGNAL cnt_inc        : std_logic;
	SIGNAL cnt_dec        : std_logic;
	SIGNAL cnt_data       : std_logic_vector(7 DOWNTO 0);
	SIGNAL cnt_empty      : std_logic;

	SIGNAL tmp_ld         : std_logic;
	SIGNAL tmp_data       : std_logic_vector(7 DOWNTO 0);

	SIGNAL ptr_inc        : std_logic;
	SIGNAL ptr_dec        : std_logic;
	SIGNAL ptr_data       : std_logic_vector(9 DOWNTO 0);

	SIGNAL sel            : std_logic_vector(1 DOWNTO 0);

	SIGNAL data_rdata_aux : std_logic;
	SIGNAL cnt_aux        : std_logic;
 
	TYPE fsm_state IS (s_fetch, s_decode, s_inc_ptr, s_dec_ptr, 
	s_inc_value_1, s_inc_value_2, 
	s_dec_value_1, s_dec_value_2, 
	s_print_value_1, s_read_value_1, 
	s_print_value_2, s_read_value_2, 
	s_store_tmp_1, s_store_tmp_2, 
	s_load_tmp, s_halt, s_other_state, 
	s_loop_begin_1, s_loop_begin_2, s_loop_begin_3, s_loop_begin_4, 
	s_loop_end_1, s_loop_end_2, s_loop_end_3, s_loop_end_4, s_loop_end_5);
 
	SIGNAL present_state : fsm_state;
	SIGNAL next_state    : fsm_state;

BEGIN
	OUT_DATA <= DATA_RDATA;

---- PC ----
pc : PROCESS (RESET, CLK)
BEGIN
	IF (RESET = '1') THEN
		pc_data <= (OTHERS => '0');
	ELSIF RISING_EDGE(CLK) THEN
		IF (pc_inc = '1') THEN
			pc_data <= pc_data + 1;
		ELSIF (pc_dec = '1') THEN
			pc_data <= pc_data - 1;
		END IF;
	END IF;
	CODE_ADDR <= pc_data;
END PROCESS;

---- CNT ----
cnt : PROCESS (RESET, CLK)
BEGIN
	IF (RESET = '1') THEN
		cnt_data <= (OTHERS => '0');
	ELSIF RISING_EDGE(CLK) THEN
		IF (cnt_inc = '1') THEN
			cnt_data <= cnt_data + 1;
		ELSIF (cnt_dec = '1') THEN
			cnt_data <= cnt_data - 1;
		END IF;
		IF (cnt_empty = '1') THEN
			cnt_data <= "00000001";
		END IF;
	END IF;
END PROCESS;

---- TMP ----
tmp : PROCESS (RESET, CLK)
BEGIN
	IF (RESET = '1') THEN
		tmp_data <= (OTHERS => '0');
	ELSIF RISING_EDGE(CLK) THEN
		IF (tmp_ld = '1') THEN
			tmp_data <= DATA_RDATA;
		END IF;
	END IF;
END PROCESS;

---- PTR ----
ptr : PROCESS (RESET, CLK)
BEGIN
	IF (RESET = '1') THEN
		ptr_data <= (OTHERS => '0');
	ELSIF RISING_EDGE(CLK) THEN
		IF (ptr_inc = '1') THEN
			ptr_data <= ptr_data + 1;
		ELSIF (ptr_dec = '1') THEN
			ptr_data <= ptr_data - 1;
		END IF;
	END IF;
	DATA_ADDR <= ptr_data;
END PROCESS;

---- MUX ----
WITH sel SELECT
DATA_WDATA <= IN_DATA WHEN "00", 
              DATA_RDATA + "00000001" WHEN "01", 
              DATA_RDATA - "00000001" WHEN "10", 
              tmp_data WHEN "11", 
(OTHERS => '0') WHEN OTHERS;

cnt_aux        <= '1' WHEN (cnt_data = "00000000") ELSE '0';
data_rdata_aux <= '1' WHEN (DATA_RDATA = "00000000") ELSE '0';

---- PRESENT_STATE ----
fsm_present_state : PROCESS (RESET, CLK)
BEGIN
	IF (RESET = '1') THEN
		present_state <= s_fetch;
	ELSIF RISING_EDGE(CLK) THEN
		IF (EN = '1') THEN
			present_state <= next_state;
		END IF;
	END IF;
END PROCESS;

---- NEXT_STATE ----
fsm_next_state : PROCESS (IN_VLD, IN_DATA, DATA_RDATA, present_state, data_rdata_aux, cnt_aux, CODE_DATA, OUT_BUSY)
BEGIN
	next_state <= s_fetch;

	CODE_EN    <= '0';
	DATA_RDWR  <= '1';
	DATA_EN    <= '0';
	IN_REQ     <= '0';
	OUT_WE     <= '0';
	pc_inc     <= '0';
	pc_dec     <= '0';
	cnt_inc    <= '0';
	cnt_dec    <= '0';
	cnt_empty  <= '0';
	tmp_ld     <= '0';
	ptr_inc    <= '0';
	ptr_dec    <= '0';
	sel        <= "01";

	CASE present_state IS
 
		---- FETCH ----
		WHEN s_fetch => 
			next_state <= s_decode;
			CODE_EN    <= '1';
 
		---- DECODE ----
		WHEN s_decode => 
			CASE CODE_DATA IS
				WHEN X"3E" => next_state <= s_inc_ptr; 				-- >
				WHEN X"3C" => next_state <= s_dec_ptr; 				-- <
				WHEN X"2B" => next_state <= s_inc_value_1;			-- +
				WHEN X"2D" => next_state <= s_dec_value_1; 			-- -
				WHEN X"5B" => next_state <= s_loop_begin_1; 			-- [
				WHEN X"5D" => next_state <= s_loop_end_1;				-- ]
				WHEN X"2E" => next_state <= s_print_value_1; 		-- .
				WHEN X"2C" => next_state <= s_read_value_1;		   -- ,
				WHEN X"24" => next_state <= s_store_tmp_1; 			-- $
				WHEN X"21" => next_state <= s_load_tmp; 				-- !
				WHEN X"00" => next_state <= s_halt; 					-- return
				WHEN OTHERS =>next_state <= s_other_state; 			-- other_state
		END CASE;
 
		---- INC_PTR ">" ----
		WHEN s_inc_ptr => 
			ptr_inc    <= '1';
			pc_inc     <= '1';
			next_state <= s_fetch;
 

		---- DEC_PTR "<" ----
		WHEN s_dec_ptr => 
			ptr_dec    <= '1';
			pc_inc     <= '1';
			next_state <= s_fetch;
 

		---- INC_VALUE "+" ----
		WHEN s_inc_value_1 => 
			DATA_EN    <= '1';
			DATA_RDWR  <= '1';
			next_state <= s_inc_value_2;

		WHEN s_inc_value_2 => 
			DATA_EN    <= '1';
			DATA_RDWR  <= '0';
			sel        <= "01";
			pc_inc     <= '1';
			next_state <= s_fetch;
 

		---- DEC_VALUE "-" ----
		WHEN s_dec_value_1 => 
			DATA_EN    <= '1';
			DATA_RDWR  <= '1';
			next_state <= s_dec_value_2;

		WHEN s_dec_value_2 => 
			DATA_EN    <= '1';
			DATA_RDWR  <= '0';
			sel        <= "10";
			pc_inc     <= '1';
			next_state <= s_fetch;
 
 
		---- LOOP_BEGIN "[" ---- 
		WHEN s_loop_begin_1 => 
			DATA_RDWR  <= '1';
			DATA_EN    <= '1';
			pc_inc     <= '1';
			next_state <= s_loop_begin_2;

		WHEN s_loop_begin_2 => 
			IF (data_rdata_aux = '1') THEN
				next_state <= s_loop_begin_3;
				CODE_EN    <= '1';
				cnt_empty  <= '1';
			ELSE
				next_state <= s_fetch;
			END IF;

		WHEN s_loop_begin_3 => 
			IF (cnt_aux = '0') THEN
				pc_inc <= '1';
				IF (CODE_DATA = X"5B") THEN
					cnt_inc <= '1';
				ELSIF (CODE_DATA = X"5D") THEN
					cnt_dec <= '1';
				END IF;
				next_state <= s_loop_begin_4;
			ELSE
				next_state <= s_fetch;
			END IF;

		WHEN s_loop_begin_4 => 
			CODE_EN    <= '1';
			next_state <= s_loop_begin_3;
			
			
			
		---- LOOP_END "]" ----
		WHEN s_loop_end_1 => 
			DATA_RDWR  <= '1';
			DATA_EN    <= '1';
			next_state <= s_loop_end_2;

		WHEN s_loop_end_2 => 
			IF (data_rdata_aux = '1') THEN
				pc_inc     <= '1';
				next_state <= s_fetch;
			ELSE
				cnt_empty  <= '1';
				pc_dec     <= '1';
				next_state <= s_loop_end_3;
			END IF;

		WHEN s_loop_end_3 => 
			CODE_EN    <= '1';
			next_state <= s_loop_end_4;

		WHEN s_loop_end_4 => 
			IF (cnt_aux = '0') THEN
				IF (CODE_DATA = X"5D") THEN
					cnt_inc <= '1';
				ELSIF (CODE_DATA = X"5B") THEN
					cnt_dec <= '1';
				END IF;
				next_state <= s_loop_end_5;
			ELSE
				next_state <= s_fetch;
			END IF;

		WHEN s_loop_end_5 => 
			next_state <= s_loop_end_3;
			IF (cnt_aux = '1') THEN
				pc_inc <= '1';
			ELSE
				pc_dec <= '1';
			END IF;
 
 
		---- PRINT_VALUE "." ----
		WHEN s_print_value_1 => 
			DATA_RDWR  <= '1';
			DATA_EN    <= '1';
			next_state <= s_print_value_2;

		WHEN s_print_value_2 => 
			IF (OUT_BUSY = '0') THEN
				next_state <= s_fetch;
				OUT_WE     <= '1';
				pc_inc     <= '1';
			ELSE
				next_state <= s_print_value_2;
			END IF;

 
		---- READ_VALUE "," ----
		WHEN s_read_value_1 => 
			next_state <= s_read_value_2;
			IN_REQ     <= '1';

		WHEN s_read_value_2 => 
			IN_REQ <= '1';
			IF (IN_VLD = '1') THEN
				next_state <= s_fetch;
				sel        <= "00";
				DATA_RDWR  <= '0';
				DATA_EN    <= '1';
				pc_inc     <= '1';
			ELSE
				next_state <= s_read_value_2;
			END IF;
 

		---- STORE_TMP "$" ----
		WHEN s_store_tmp_1 => 
			DATA_RDWR  <= '1';
			DATA_EN    <= '1'; 
			next_state <= s_store_tmp_2;

		WHEN s_store_tmp_2 => 
			tmp_ld     <= '1';
			pc_inc     <= '1';
			next_state <= s_fetch;

 
		---- LOAD_TMP "!" ---- 
		WHEN s_load_tmp => 
			DATA_RDWR  <= '0';
			DATA_EN    <= '1';
			sel        <= "11";
			pc_inc     <= '1';
			next_state <= s_fetch;
 

		---- S_OTHER_STATE "Comment/Not defined" ----
		WHEN s_other_state => 
			pc_inc     <= '1';
			next_state <= s_fetch;
 
 
		---- HALT "return" ----
		WHEN s_halt => 
			next_state <= s_halt;
 
	END CASE;
END PROCESS;
END behavioral;