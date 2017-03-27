library IEEE; 
use IEEE.std_logic_1164.all;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_unsigned.all;


entity ledc8x8 is
port(                 
	SMCLK, RESET: in std_logic;
	ROW, LED: out std_logic_vector(0 to 7)
);
end entity;


architecture arch of ledc8x8 is
	  signal gen_clk: STD_LOGIC_VECTOR (23 downto 0);
	  signal rows: STD_LOGIC_VECTOR (7 downto 0);
	
begin
	gen_smclk: process(SMCLK, RESET)
	begin 
	
		if RESET = '1' then
			gen_clk <= ( others => '0' );
			
		elsif SMCLK'event and SMCLK = '1' then
			gen_clk <= gen_clk + 1;
		end if;
 	
	end process;
	

	rotate: process(SMCLK,RESET,rows)
	begin
		if RESET = '1' then
			rows <= "10000000"; -- prvy
		elsif (SMCLK = '1' and SMCLK'event) then
			rows <= rows(0) & rows(7 downto 1); --konkatenancia
		end if;
		
	end process;
	
	letters :process(rows, gen_clk)
	begin
		if gen_clk(23) = '1' then 
			case rows is
				when "10000000" => LED <= "00001111";
				when "01000000" => LED <= "11101111";
				when "00100000" => LED <= "11101111";
				when "00010000" => LED <= "01101111";
				when "00001000" => LED <= "10011111";
				when "00000100" => LED <= "11111111";
				when "00000010" => LED <= "11111111";
				when "00000001" => LED <= "11111111";
				when others     => LED <= "11111111";
			end case;
		else
			case rows is
				when "10000000" => LED <= "01101111";
				when "01000000" => LED <= "01101111";
				when "00100000" => LED <= "01101111";
				when "00010000" => LED <= "01101111";
				when "00001000" => LED <= "10011111";
				when "00000100" => LED <= "11111111";
				when "00000010" => LED <= "11111111";
				when "00000001" => LED <= "11111111";
				when others     => LED <= "11111111";
			end case;
		end if;
    ROW <= rows;
	end process;
end arch;	