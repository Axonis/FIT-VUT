
SET serveroutput ON;

DROP TABLE Bug      			CASCADE CONSTRAINTS;
DROP TABLE Modul    			CASCADE CONSTRAINTS;
DROP TABLE Uzivatel 			CASCADE CONSTRAINTS;
DROP TABLE Ticket   			CASCADE CONSTRAINTS;
DROP TABLE Patch    			CASCADE CONSTRAINTS;
DROP TABLE Test     			CASCADE CONSTRAINTS;
DROP TABLE Charakterizuje   	CASCADE CONSTRAINTS;
DROP TABLE Zranitelnost     	CASCADE CONSTRAINTS;
DROP TABLE Programator      	CASCADE CONSTRAINTS;
DROP TABLE Bezny_uzivatel   	CASCADE CONSTRAINTS;
DROP TABLE Programovaci_jazyk 	CASCADE CONSTRAINTS;
DROP TABLE Ovlada             CASCADE CONSTRAINTS;
DROP SEQUENCE ID_ticket_sequence;
DROP SEQUENCE ID_bug_sequence;
DROP MATERIALIZED VIEW Bug_view;


CREATE TABLE Modul (
  ID_modul int NOT NULL,
  chybovost float DEFAULT NULL,
  datum_poslednej_upravy date DEFAULT NULL,
  Nazov_jazyka NVARCHAR2(64),
  
  Nickname_zodpovedny NVARCHAR2(64),
  
  PRIMARY KEY (ID_modul)
);

CREATE TABLE Bug (
  ID_bug int NOT NULL,
  Typ NVARCHAR2(128), /*Arithmetic, Logic, Syntax, Resource, Multi-threading, Interfacing, Performance, Not a bug*/
  Zavaznost NVARCHAR2(128), /*Low, Medium, High*/
  ID_modul int NOT NULL,
  ID_patch int, 

  PRIMARY KEY (ID_bug)
);

CREATE TABLE Ticket (
  ID_ticket int NOT NULL,
  Datum_podania date NOT NULL,
  Stav NVARCHAR2(128), /*New, Assigned, Open, Need Info, Closed*/
  Nickname_prideleny NVARCHAR2(64),
  Nickname_vyvoreny NVARCHAR2(64) NOT NULL,

  PRIMARY KEY(ID_ticket)
);

CREATE TABLE Patch(
  ID_patch int NOT NULL,
  Schvalenie int,
  Datum_vydania date,
  Datum_zavedenia date,

  Nickname_vydany NVARCHAR2(64),

  PRIMARY KEY (ID_patch)
);

CREATE TABLE Test (
  ID_test int NOT NULL,
  Datum_zaciatku DATE,
  Datum_konca DATE,
  Hodnotenie float,

  ID_patch int,
  Nickname_schvaleny NVARCHAR2(64),

  PRIMARY KEY (ID_test)
);


CREATE TABLE Uzivatel (

  Nickname NVARCHAR2(64) NOT NULL,
  Meno NVARCHAR2(128) NOT NULL,
  Vek int,

  PRIMARY KEY (Nickname)
);

CREATE TABLE Bezny_uzivatel (
  Nickname NVARCHAR2(64) NOT NULL REFERENCES Uzivatel(Nickname),
  Odmena INT,
  Karma INT,
  
  PRIMARY KEY (Nickname)
);

CREATE TABLE Programator (
  Nickname NVARCHAR2(64) NOT NULL REFERENCES Uzivatel(Nickname),
  Produkt NVARCHAR2(64),
  Rank INT,
  
  PRIMARY KEY (Nickname) 
);

CREATE TABLE Programovaci_jazyk (
  Nazov_jazyka NVARCHAR2(64) NOT NULL,
  
  PRIMARY KEY(Nazov_jazyka)
);

CREATE TABLE Ovlada (
  Nickname NVARCHAR2(64) NOT NULL,
  Nazov_jazyka NVARCHAR2(64) NOT NULL,
  
  Skusenost NVARCHAR2(64),/*Beginner, Advanced, Expert*/
  
  PRIMARY KEY (Nickname,Nazov_jazyka)
);


CREATE TABLE Charakterizuje (
  ID_ticket int NOT NULL,
  ID_bug int NOT NULL,

  Datum_vyskytu DATE,
  Frekvencia_vyskytu INT,
  Popis_vyskytu NVARCHAR2(1024),
  Popis_problemu NVARCHAR2(1024),

  PRIMARY KEY (ID_ticket,ID_bug)
);

CREATE TABLE Zranitelnost(
  ID_vulnerablity INT NOT NULL REFERENCES Bug(ID_bug),
  Miera_nebezpecenstva NVARCHAR2(64), /*Low, Medium, High, Critical*/
  Moznost_zneuzitia NVARCHAR2(64),
  
  PRIMARY KEY (ID_vulnerablity)   
);

ALTER TABLE Modul ADD FOREIGN KEY (Nickname_zodpovedny) REFERENCES Uzivatel;
ALTER TABLE Bug ADD FOREIGN KEY (ID_modul) REFERENCES Modul;
ALTER TABLE Bug ADD FOREIGN KEY (ID_patch) REFERENCES Patch;
ALTER TABLE Ticket ADD FOREIGN KEY (Nickname_prideleny) REFERENCES Uzivatel(Nickname);
ALTER TABLE Ticket ADD FOREIGN KEY (Nickname_vyvoreny) REFERENCES Uzivatel(Nickname);

ALTER TABLE Patch ADD FOREIGN KEY (Nickname_vydany) REFERENCES Uzivatel(Nickname);
ALTER TABLE Test ADD FOREIGN KEY (ID_patch) REFERENCES Patch(ID_patch);
ALTER TABLE Test ADD FOREIGN KEY (Nickname_schvaleny) REFERENCES Uzivatel(Nickname);

ALTER TABLE Charakterizuje ADD FOREIGN KEY (ID_ticket) REFERENCES Ticket(ID_ticket);
ALTER TABLE Charakterizuje ADD FOREIGN KEY (ID_bug) REFERENCES Bug(ID_bug);

ALTER TABLE Ovlada ADD FOREIGN KEY (Nickname) REFERENCES Uzivatel;
ALTER TABLE Ovlada ADD FOREIGN KEY (Nazov_jazyka) REFERENCES Programovaci_jazyk;

ALTER TABLE Modul ADD FOREIGN KEY (Nazov_jazyka) REFERENCES Programovaci_jazyk;

ALTER TABLE Modul ADD CONSTRAINTS chk_chybovost CHECK (chybovost >= 0 AND chybovost <= 100);
ALTER TABLE Test ADD CONSTRAINTS chk_hodnotenie CHECK (Hodnotenie >= 0 AND Hodnotenie <= 100);


/* Sequence + trigger pre auto increment */
CREATE SEQUENCE ID_ticket_sequence
  START WITH 1
  INCREMENT BY 1
  CACHE 100;

CREATE OR REPLACE TRIGGER Ticket_number 
BEFORE INSERT ON Ticket 
FOR EACH ROW

BEGIN
  SELECT ID_ticket_sequence.NEXTVAL
  INTO   :new.ID_ticket
  FROM   dual;
END;
/

/* Sequence + trigger pre auto increment */
CREATE SEQUENCE ID_bug_sequence
  START WITH 1
  INCREMENT BY 1
  CACHE 100;
  
CREATE OR REPLACE TRIGGER Bug_number 
BEFORE INSERT ON Bug 
FOR EACH ROW

BEGIN
  SELECT ID_bug_sequence.NEXTVAL
  INTO   :new.ID_bug
  FROM   dual;
END;
/
  
CREATE OR REPLACE TRIGGER Trigger_chybovost
AFTER INSERT ON Bug
DECLARE Modul_chyb FLOAT;
Vs_chyb FLOAT;
Vysl FLOAT;
Cislo_bugu INT;
Cislo_modulu INT;

BEGIN 
  SELECT MAX(ID_bug) INTO Cislo_bugu FROM Bug;
  SELECT ID_modul INTO Cislo_modulu FROM Bug WHERE ID_bug = Cislo_bugu;
  SELECT COUNT(ID_modul) INTO Modul_chyb FROM Bug WHERE ID_modul = Cislo_modulu GROUP BY ID_modul;
  SELECT SUM(tmp) INTO Vs_chyb FROM (SELECT COUNT(ID_modul)AS tmp FROM Bug GROUP BY ID_modul);
  Vysl := Modul_chyb / Vs_chyb * 100;
  UPDATE Modul SET Chybovost = Vysl WHERE ID_modul = Cislo_modulu;
  
END;
/
      SELECT  * from Uzivatel Join Ovlada on Uzivatel.NICKNAME = Ovlada.NICKNAME ;


CREATE OR REPLACE PROCEDURE vyuzivanie_prog_jazyku AS
  cursor c_jazyky is SELECT * FROM Programovaci_jazyk ;
  r_jazyk c_jazyky%ROWTYPE;
  pocet_programatorov int;
  jazyk_ovlada INT;
  tmp FLOAT;
  
  begin
  
  SELECT  COUNT(unique (Ovlada.NICKNAME)) INTO pocet_programatorov from Uzivatel Join Ovlada on Uzivatel.NICKNAME = Ovlada.NICKNAME;
           DBMS_OUTPUT.PUT_LINE('Kolko percent uzivatelov ovlada dany jazyk');
  open c_jazyky;
  loop
   fetch c_jazyky into r_jazyk;
      exit when c_jazyky%NOTFOUND;

      SELECT  COUNT(unique (Ovlada.NICKNAME)) into jazyk_ovlada from Uzivatel Join Ovlada on Uzivatel.NICKNAME = Ovlada.NICKNAME WHERE Ovlada.NAZOV_JAZYKA = r_jazyk.Nazov_jazyka;
         tmp := jazyk_ovlada / pocet_programatorov * 100;
         DBMS_OUTPUT.PUT(r_jazyk.Nazov_jazyka);
         DBMS_OUTPUT.PUT('  >>  ');
         DBMS_OUTPUT.PUT_LINE(ROUND(tmp, 2));

  end loop;
  close c_jazyky;
  
END;
/

exec vyuzivanie_prog_jazyku;

INSERT INTO Uzivatel (Nickname, Meno, Vek) VALUES('xxKebabmajsterxx', 'Peter Jablko', '21');
INSERT INTO Uzivatel (Nickname, Meno, Vek) VALUES('AndreDankojeLegenda', 'Juraj Zemiak', '22');
INSERT INTO Uzivatel (Nickname, Meno, Vek) VALUES('MirrorMaster12', 'Jan Penaze', '23');
INSERT INTO Uzivatel (Nickname, Meno, Vek) VALUES('Cyborg13', 'Ondrej Vychodnar', '27');
INSERT INTO Uzivatel (Nickname, Meno, Vek) VALUES('Knedla8', 'Michal Krivozub', '19');
INSERT INTO Uzivatel (Nickname, Meno, Vek) VALUES('Destroyer-ultimate', 'Daniel Drevo', '33');
INSERT INTO Uzivatel (Nickname, Meno, Vek) VALUES('Ahojakosamas', 'Jozef Koleso', '28');
INSERT INTO Uzivatel (Nickname, Meno, Vek) VALUES('Iliketurtles', 'Adrian Karfiol', '31');
INSERT INTO Uzivatel (Nickname, Meno, Vek) VALUES('Whysoserious', 'Tomas Exekutor', '30');
INSERT INTO Uzivatel (Nickname, Meno, Vek) VALUES('NovaZilina', 'Silvester Akalas', '18');

INSERT INTO Programator (Nickname, Produkt, Rank) VALUES('xxKebabmajsterxx', 'OpenStack', '8');
INSERT INTO Programator (Nickname, Produkt, Rank) VALUES('AndreDankojeLegenda', 'OpenStack', '6');
INSERT INTO Programator (Nickname, Produkt, Rank) VALUES('MirrorMaster12', 'Kernel', '2');
INSERT INTO Programator (Nickname, Produkt, Rank) VALUES('Ahojakosamas', 'Kernel', '9');
INSERT INTO Programator (Nickname, Produkt, Rank) VALUES('NovaZilina', 'Struska', '10');

INSERT INTO Bezny_uzivatel (Nickname, Odmena, Karma) VALUES('Cyborg13', null, null);
INSERT INTO Bezny_uzivatel (Nickname, Odmena, Karma) VALUES('Destroyer-ultimate', '10000', '1000');
INSERT INTO Bezny_uzivatel (Nickname, Odmena, Karma) VALUES('Whysoserious', '56464', '56');

INSERT INTO Programovaci_jazyk (Nazov_jazyka) VALUES ('C');
INSERT INTO Programovaci_jazyk (Nazov_jazyka) VALUES ('D');
INSERT INTO Programovaci_jazyk (Nazov_jazyka) VALUES ('GO');
INSERT INTO Programovaci_jazyk (Nazov_jazyka) VALUES ('C++');
INSERT INTO Programovaci_jazyk (Nazov_jazyka) VALUES ('C#');
INSERT INTO Programovaci_jazyk (Nazov_jazyka) VALUES ('Java');
INSERT INTO Programovaci_jazyk (Nazov_jazyka) VALUES ('JavaScript');
INSERT INTO Programovaci_jazyk (Nazov_jazyka) VALUES ('Python');

INSERT INTO Modul (ID_modul, chybovost, datum_poslednej_upravy, Nickname_zodpovedny, Nazov_jazyka) VALUES('1', '1,25', '21-12-2016', 'NovaZilina', 'Java');
INSERT INTO Modul (ID_modul, chybovost, datum_poslednej_upravy, Nickname_zodpovedny, Nazov_jazyka) VALUES('2', '2,83', '24-03-2017', 'MirrorMaster12', 'C');
INSERT INTO Modul (ID_modul, chybovost, datum_poslednej_upravy, Nickname_zodpovedny, Nazov_jazyka) VALUES('3', '1,14', '17-02-2017', 'xxKebabmajsterxx', 'GO');
INSERT INTO Modul (ID_modul, chybovost, datum_poslednej_upravy, Nickname_zodpovedny, Nazov_jazyka) VALUES('4', '3,84', '16-11-2015', 'AndreDankojeLegenda', 'Python');
INSERT INTO Modul (ID_modul, chybovost, datum_poslednej_upravy, Nickname_zodpovedny, Nazov_jazyka) VALUES('5', '0,12', '14-01-2016', 'xxKebabmajsterxx', 'C++');

INSERT INTO Patch (ID_patch, Schvalenie, Datum_vydania, Datum_zavedenia, Nickname_vydany) VALUES('20170217', '0', '03-12-2017', '', 'xxKebabmajsterxx');
INSERT INTO Patch (ID_patch, Schvalenie, Datum_vydania, Datum_zavedenia, Nickname_vydany) VALUES('20170219', '1', '07-11-2017', '05-12-2017', 'Ahojakosamas');
INSERT INTO Patch (ID_patch, Schvalenie, Datum_vydania, Datum_zavedenia, Nickname_vydany) VALUES('20170218', '1', '06-10-2017', '06-12-2017', 'Cyborg13');
INSERT INTO Patch (ID_patch, Schvalenie, Datum_vydania, Datum_zavedenia, Nickname_vydany) VALUES('20170216', '1', '05-09-2017', '07-12-2017', 'Knedla8');
INSERT INTO Patch (ID_patch, Schvalenie, Datum_vydania, Datum_zavedenia, Nickname_vydany) VALUES('20170212', '1', '04-08-2017', '08-12-2017', 'Knedla8');

INSERT INTO Bug (ID_modul, Typ, Zavaznost, ID_patch) VALUES('1', 'Logic', 'High', '20170219');
INSERT INTO Bug (ID_modul, Typ, Zavaznost, ID_patch) VALUES('2', 'Syntax', 'Medium', '20170216');
INSERT INTO Bug (ID_modul, Typ, Zavaznost, ID_patch) VALUES('3', 'Not a bug', 'Low', '20170218');
INSERT INTO Bug (ID_modul, Typ, Zavaznost) VALUES('4', 'Interfacing', 'Medium');
INSERT INTO Bug (ID_modul, Typ, Zavaznost) VALUES('5', 'Resource', 'Low');
INSERT INTO Bug (ID_modul, Typ, Zavaznost) VALUES('1', 'Logic', 'High');
INSERT INTO Bug (ID_modul, Typ, Zavaznost) VALUES('2', 'Syntax', 'Medium');
INSERT INTO Bug (ID_modul, Typ, Zavaznost) VALUES('3', 'Logic', 'High');
INSERT INTO Bug (ID_modul, Typ, Zavaznost) VALUES('1', 'Resource', 'High');

INSERT INTO Zranitelnost (ID_vulnerablity, Miera_nebezpecenstva, Moznost_zneuzitia) VALUES('6', 'High', 'Modul');
INSERT INTO Zranitelnost (ID_vulnerablity, Miera_nebezpecenstva, Moznost_zneuzitia) VALUES('8', 'Critical', 'System');

INSERT INTO Ticket (Datum_podania, Stav, Nickname_prideleny, Nickname_vyvoreny) VALUES('23-03-2017', 'Open', 'xxKebabmajsterxx', 'Cyborg13');
INSERT INTO Ticket (Datum_podania, Stav, Nickname_prideleny, Nickname_vyvoreny) VALUES('13-02-2017', 'Assigned', 'MirrorMaster12', 'Knedla8');
INSERT INTO Ticket (Datum_podania, Stav, Nickname_prideleny, Nickname_vyvoreny) VALUES('23-01-2017', 'New', '', 'Cyborg13');
INSERT INTO Ticket (Datum_podania, Stav, Nickname_prideleny, Nickname_vyvoreny) VALUES('02-11-2017', 'Closed', 'Ahojakosamas', 'Knedla8');
INSERT INTO Ticket (Datum_podania, Stav, Nickname_prideleny, Nickname_vyvoreny) VALUES('24-03-2017', 'Need Info', 'Destroyer-ultimate', 'Cyborg13');

INSERT INTO Charakterizuje (ID_ticket, ID_bug, Datum_vyskytu, Frekvencia_vyskytu, Popis_vyskytu, Popis_problemu) VALUES ('1', '8', '21-03-2016', '10', '1.Install System, 2.Run cmd command', 'Kernel Panic');
INSERT INTO Charakterizuje (ID_ticket, ID_bug, Datum_vyskytu, Frekvencia_vyskytu, Popis_vyskytu, Popis_problemu) VALUES ('2', '7', '13-03-2015', '7', '1.Install System, 2.Run cmd command', 'Kernel Panic');
INSERT INTO Charakterizuje (ID_ticket, ID_bug, Datum_vyskytu, Frekvencia_vyskytu, Popis_vyskytu, Popis_problemu) VALUES ('2', '3', '11-03-2014', '10', '1.Install System, 2.Run cmd command', 'Kernel Panic');
INSERT INTO Charakterizuje (ID_ticket, ID_bug, Datum_vyskytu, Frekvencia_vyskytu, Popis_vyskytu, Popis_problemu) VALUES ('3', '4', '01-04-2017', '7', '1.Install System, 2.Run cmd command', 'Kernel Panic');
INSERT INTO Charakterizuje (ID_ticket, ID_bug, Datum_vyskytu, Frekvencia_vyskytu, Popis_vyskytu, Popis_problemu) VALUES ('1', '5', '06-05-2017', '8', '1.Install System, 2.Run cmd command', 'Kernel Panic');
INSERT INTO Charakterizuje (ID_ticket, ID_bug, Datum_vyskytu, Frekvencia_vyskytu, Popis_vyskytu, Popis_problemu) VALUES ('2', '6', '07-03-2017', '9', '1.Install System, 2.Run cmd command', 'Kernel Panic');
INSERT INTO Charakterizuje (ID_ticket, ID_bug, Datum_vyskytu, Frekvencia_vyskytu, Popis_vyskytu, Popis_problemu) VALUES ('3', '9', '08-03-2017', '1', '1.Install System, 2.Run cmd command', 'Kernel Panic');

INSERT INTO Test (ID_test, Datum_zaciatku, Datum_konca, Hodnotenie, ID_patch, Nickname_schvaleny) VALUES('20170102', '01-03-2017', '01-03-2017', '98,6', '20170217', 'xxKebabmajsterxx');
INSERT INTO Test (ID_test, Datum_zaciatku, Datum_konca, Hodnotenie, ID_patch, Nickname_schvaleny) VALUES('20170214', '01-04-2017', '01-05-2017', '91,8', '20170219', null);
INSERT INTO Test (ID_test, Datum_zaciatku, Datum_konca, Hodnotenie, ID_patch, Nickname_schvaleny) VALUES('20170255', '01-04-2017', null, null, '20170219', null);
UPDATE Test SET Datum_konca = '03-04-2017' WHERE ID_test = 20170255;
INSERT INTO Test (ID_test, Datum_zaciatku, Datum_konca, Hodnotenie, ID_patch, Nickname_schvaleny) VALUES('20170215', '02-03-2017', '05-03-2017', '100,0', '20170218', null);
INSERT INTO Test (ID_test, Datum_zaciatku, Datum_konca, Hodnotenie, ID_patch, Nickname_schvaleny) VALUES('20170216', '07-01-2017', '07-05-2017', '100,0', '20170216', null);
INSERT INTO Test (ID_test, Datum_zaciatku, Datum_konca, Hodnotenie, ID_patch, Nickname_schvaleny) VALUES('20170217', '13-03-2017', '13-03-2018', '9,6', '20170212', 'Ahojakosamas');

INSERT INTO Ovlada (Nickname, Nazov_jazyka, Skusenost) VALUES ('Destroyer-ultimate' , 'Java', 'Beginner');
INSERT INTO Ovlada (Nickname, Nazov_jazyka, Skusenost) VALUES ('Destroyer-ultimate' , 'C++', 'Advanced');
INSERT INTO Ovlada (Nickname, Nazov_jazyka, Skusenost) VALUES ('Whysoserious' , 'C', 'Expert');
INSERT INTO Ovlada (Nickname, Nazov_jazyka, Skusenost) VALUES ('Whysoserious' , 'GO', 'Advanced');
INSERT INTO Ovlada (Nickname, Nazov_jazyka, Skusenost) VALUES ('AndreDankojeLegenda' , 'Java', 'Beginner');
INSERT INTO Ovlada (Nickname, Nazov_jazyka, Skusenost) VALUES ('AndreDankojeLegenda' , 'Python', 'Advanced');
INSERT INTO Ovlada (Nickname, Nazov_jazyka, Skusenost) VALUES ('Ahojakosamas', 'D', 'Advanced');
INSERT INTO Ovlada (Nickname, Nazov_jazyka, Skusenost) VALUES ('MirrorMaster12', 'C', 'Advanced');
INSERT INTO Ovlada (Nickname, Nazov_jazyka, Skusenost) VALUES ('MirrorMaster12', 'C++', 'Advanced');
INSERT INTO Ovlada (Nickname, Nazov_jazyka, Skusenost) VALUES ('MirrorMaster12', 'D', 'Advanced');
INSERT INTO Ovlada (Nickname, Nazov_jazyka, Skusenost) VALUES ('NovaZilina', 'D', 'Beginner');
INSERT INTO Ovlada (Nickname, Nazov_jazyka, Skusenost) VALUES ('NovaZilina', 'Java', 'Expert');
INSERT INTO Ovlada (Nickname, Nazov_jazyka, Skusenost) VALUES ('xxKebabmajsterxx', 'C++', 'Expert');
INSERT INTO Ovlada (Nickname, Nazov_jazyka, Skusenost) VALUES ('xxKebabmajsterxx', 'GO', 'Beginner');

/* Test modul trigger */
INSERT INTO Modul (ID_modul, chybovost, datum_poslednej_upravy, Nickname_zodpovedny, Nazov_jazyka) VALUES('6', '0,00', '14-06-2016', 'NovaZilina', 'D');

/*2 JOIN - Mena programatorov s rankom vyssim ako 5, ktori schvalili testy s hodnotenim nizsim ako 90*/
SELECT Test.Nickname_schvaleny 
FROM Test 
JOIN Programator ON Test.Nickname_schvaleny = Programator.Nickname 
WHERE Test.Hodnotenie <= 90 AND Programator.Rank > 5;

/*2 JOIN + IN - Aka je zavaznost bugov ktore su zatial nepridelene*/
SELECT Zavaznost 
FROM BUG 
WHERE ID_bug IN 
(SELECT ID_bug 
	FROM Ticket 
	JOIN Charakterizuje ON Ticket.ID_ticket = Charakterizuje.ID_ticket 
	WHERE Stav = 'New'
);

/*2 JOIN - Aky rank ma uzivatel ktory zodpoveda za modul cislo 2*/
SELECT Nickname_zodpovedny, Rank 
FROM Programator 
JOIN Modul ON Programator.Nickname = Modul.Nickname_zodpovedny 
WHERE ID_modul = 2;

/*3 JOIN - Aky patch opravil zranitelnost bugu 2190732*/
SELECT Patch.ID_patch 
FROM Bug 
JOIN Patch ON Bug.ID_patch = Patch.ID_patch 
JOIN Zranitelnost ON Zranitelnost.ID_vulnerablity = Bug.ID_bug 
WHERE ID_vulnerablity = 2190732;

/*3 JOIN + IN - Na akych bugoch pracuje dany programator*/
SELECT ID_Bug 
FROM Bug 
WHERE ID_bug IN 
(SELECT ID_bug 
	FROM Ticket 
	JOIN Charakterizuje ON Ticket.ID_ticket = Charakterizuje.ID_ticket 
	WHERE Ticket.Nickname_prideleny = 'xxKebabmajsterxx'
);

/*3 JOIN + EXISTS - Ktore zranitelnosti sa testuju*/
SELECT ID_vulnerablity 
FROM Zranitelnost 
WHERE EXISTS 
(SELECT ID_bug 
	FROM Test JOIN Patch ON Test.ID_patch = Patch.ID_patch 
	JOIN Bug ON Patch.ID_patch = Bug.ID_patch
);

/*GROUP BY, AGGREGATE COUNT, ORDER BY - Pocet patchov vydanych jednotlivymi uzivatelmi*/
SELECT Nickname_vydany, COUNT(ID_patch) 
FROM Patch 
GROUP BY Nickname_vydany 
ORDER BY COUNT(ID_patch) 
DESC; 

/*GROUP BY, AGGREGATE COUNT, MAX, ORDER BY - Ktore moduly maju kolko bugov*/
SELECT modul_id, bug_cnt 
FROM 
(SELECT Modul.ID_modul AS modul_id,COUNT(ID_bug) AS bug_cnt 
	FROM Bug 
	JOIN Modul ON Bug.ID_modul = Modul.ID_modul 
	GROUP BY Modul.ID_modul
) 
ORDER BY bug_cnt 
DESC;

/*GROUP BY, AGGREGATE COUNT - Pocet ticketov k bugom podanym pred rokom 2017*/
SELECT ID_bug, COUNT(ID_ticket) 
FROM Charakterizuje 
WHERE Datum_vyskytu <= '01/01/2017' 
GROUP BY Charakterizuje.ID_bug;

/* Pristupove prava */
GRANT ALL ON Test TO xtomas32;
GRANT ALL ON Uzivatel TO xtomas32;
GRANT ALL ON Programator TO xtomas32;
GRANT ALL ON Bezny_uzivatel TO xtomas32;
GRANT ALL ON Bug TO xtomas32;
GRANT ALL ON Ticket TO xtomas32;
GRANT ALL ON Programovaci_jazyk TO xtomas32;
GRANT ALL ON Zranitelnost TO xtomas32;
GRANT ALL ON Charakterizuje TO xtomas32;
GRANT ALL ON Ovlada TO xtomas32;
GRANT ALL ON Modul TO xtomas32;
GRANT ALL ON Patch TO xtomas32;


CREATE MATERIALIZED VIEW LOG ON Bug WITH PRIMARY KEY,ROWID(ID_modul)INCLUDING NEW VALUES; 
CREATE MATERIALIZED VIEW Bug_view
CACHE BUILD IMMEDIATE REFRESH FAST ON COMMIT ENABLE QUERY REWRITE
  AS SELECT ID_modul as "Modul", count(ID_bug) as "Pocet bugov v danom module" FROM Bug GROUP BY ID_modul;
GRANT ALL ON Bug_view TO xtomas32;


SELECT * from Bug_view;
INSERT INTO Bug (ID_modul, Typ, Zavaznost, ID_patch) VALUES('3', 'Kernel Panic', 'Low', '20170218');
COMMIT;
SELECT * from Bug_view;


exec vyuzivanie_prog_jazyku();
