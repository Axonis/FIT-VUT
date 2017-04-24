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
  
  Skusenost NVARCHAR2(64),
  
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

INSERT INTO Modul (ID_modul, chybovost, datum_poslednej_upravy, Nickname_zodpovedny) VALUES('1', '1,25', '21-12-2016', 'NovaZilina');
INSERT INTO Modul (ID_modul, chybovost, datum_poslednej_upravy, Nickname_zodpovedny) VALUES('2', '2,83', '24-03-2017', 'MirrorMaster12');
INSERT INTO Modul (ID_modul, chybovost, datum_poslednej_upravy, Nickname_zodpovedny) VALUES('3', '1,14', '17-02-2017', 'xxKebabmajsterxx');
INSERT INTO Modul (ID_modul, chybovost, datum_poslednej_upravy, Nickname_zodpovedny) VALUES('4', '3,84', '16-11-2015', 'AndreDankojeLegenda');
INSERT INTO Modul (ID_modul, chybovost, datum_poslednej_upravy, Nickname_zodpovedny) VALUES('5', '0,12', '14-01-2016', 'xxKebabmajsterxx');

INSERT INTO Patch (ID_patch, Schvalenie, Datum_vydania, Datum_zavedenia, Nickname_vydany) VALUES('20170217', '0', '03-12-2017', '', 'xxKebabmajsterxx');
INSERT INTO Patch (ID_patch, Schvalenie, Datum_vydania, Datum_zavedenia, Nickname_vydany) VALUES('20170219', '1', '07-11-2017', '05-12-2017', 'Ahojakosamas');
INSERT INTO Patch (ID_patch, Schvalenie, Datum_vydania, Datum_zavedenia, Nickname_vydany) VALUES('20170218', '1', '06-10-2017', '06-12-2017', 'Cyborg13');
INSERT INTO Patch (ID_patch, Schvalenie, Datum_vydania, Datum_zavedenia, Nickname_vydany) VALUES('20170216', '1', '05-09-2017', '07-12-2017', 'Knedla8');
INSERT INTO Patch (ID_patch, Schvalenie, Datum_vydania, Datum_zavedenia, Nickname_vydany) VALUES('20170212', '1', '04-08-2017', '08-12-2017', 'Knedla8');

INSERT INTO Bug (ID_bug, ID_modul, Typ, Zavaznost, ID_patch) VALUES('2190732', '1', 'Logic', 'High', '20170219');
INSERT INTO Bug (ID_bug, ID_modul, Typ, Zavaznost, ID_patch) VALUES('2190746', '2', 'Syntax', 'Medium', '20170216');
INSERT INTO Bug (ID_bug, ID_modul, Typ, Zavaznost, ID_patch) VALUES('2190789', '3', 'Not a bug', 'Low', '20170218');
INSERT INTO Bug (ID_bug, ID_modul, Typ, Zavaznost) VALUES('2190719', '4', 'Interfacing', 'Medium');
INSERT INTO Bug (ID_bug, ID_modul, Typ, Zavaznost) VALUES('2190729', '5', 'Resource', 'Low');
INSERT INTO Bug (ID_bug, ID_modul, Typ, Zavaznost) VALUES('2165646', '1', 'Logic', 'High');
INSERT INTO Bug (ID_bug, ID_modul, Typ, Zavaznost) VALUES('2132213', '2', 'Syntax', 'Medium');
INSERT INTO Bug (ID_bug, ID_modul, Typ, Zavaznost) VALUES('6897983', '3', 'Logic', 'High');
INSERT INTO Bug (ID_bug, ID_modul, Typ, Zavaznost) VALUES('8978653', '1', 'Resource', 'High');

INSERT INTO Zranitelnost (ID_vulnerablity, Miera_nebezpecenstva, Moznost_zneuzitia) VALUES('2190732', 'High', 'Modul');
INSERT INTO Zranitelnost (ID_vulnerablity, Miera_nebezpecenstva, Moznost_zneuzitia) VALUES('2190746', 'Critical', 'System');

INSERT INTO Ticket (ID_ticket, Datum_podania, Stav, Nickname_prideleny, Nickname_vyvoreny) VALUES('46864', '23-03-2017', 'Open', 'xxKebabmajsterxx', 'Cyborg13');
INSERT INTO Ticket (ID_ticket, Datum_podania, Stav, Nickname_prideleny, Nickname_vyvoreny) VALUES('48864', '13-02-2017', 'Assigned', 'MirrorMaster12', 'Knedla8');
INSERT INTO Ticket (ID_ticket, Datum_podania, Stav, Nickname_prideleny, Nickname_vyvoreny) VALUES('41864', '23-01-2017', 'New', '', 'Cyborg13');
INSERT INTO Ticket (ID_ticket, Datum_podania, Stav, Nickname_prideleny, Nickname_vyvoreny) VALUES('41865', '02-11-2017', 'Closed', 'Ahojakosamas', 'Knedla8');
INSERT INTO Ticket (ID_ticket, Datum_podania, Stav, Nickname_prideleny, Nickname_vyvoreny) VALUES('41868', '24-03-2017', 'Need Info', 'Destroyer-ultimate', 'Cyborg13');

INSERT INTO Charakterizuje (ID_ticket, ID_bug, Datum_vyskytu, Frekvencia_vyskytu, Popis_vyskytu, Popis_problemu) VALUES ('46864', '2190732', '21-03-2016', '10', '1.Install System, 2.Run cmd command', 'Kernel Panic');
INSERT INTO Charakterizuje (ID_ticket, ID_bug, Datum_vyskytu, Frekvencia_vyskytu, Popis_vyskytu, Popis_problemu) VALUES ('48864', '2190732', '13-03-2015', '7', '1.Install System, 2.Run cmd command', 'Kernel Panic');
INSERT INTO Charakterizuje (ID_ticket, ID_bug, Datum_vyskytu, Frekvencia_vyskytu, Popis_vyskytu, Popis_problemu) VALUES ('48864', '2190746', '11-03-2014', '10', '1.Install System, 2.Run cmd command', 'Kernel Panic');
INSERT INTO Charakterizuje (ID_ticket, ID_bug, Datum_vyskytu, Frekvencia_vyskytu, Popis_vyskytu, Popis_problemu) VALUES ('41864', '2190746', '01-04-2017', '7', '1.Install System, 2.Run cmd command', 'Kernel Panic');
INSERT INTO Charakterizuje (ID_ticket, ID_bug, Datum_vyskytu, Frekvencia_vyskytu, Popis_vyskytu, Popis_problemu) VALUES ('46864', '2165646', '06-05-2017', '8', '1.Install System, 2.Run cmd command', 'Kernel Panic');
INSERT INTO Charakterizuje (ID_ticket, ID_bug, Datum_vyskytu, Frekvencia_vyskytu, Popis_vyskytu, Popis_problemu) VALUES ('48864', '2132213', '07-03-2017', '9', '1.Install System, 2.Run cmd command', 'Kernel Panic');
INSERT INTO Charakterizuje (ID_ticket, ID_bug, Datum_vyskytu, Frekvencia_vyskytu, Popis_vyskytu, Popis_problemu) VALUES ('41864', '6897983', '08-03-2017', '1', '1.Install System, 2.Run cmd command', 'Kernel Panic');

INSERT INTO Test (ID_test, Datum_zaciatku, Datum_konca, Hodnotenie, ID_patch, Nickname_schvaleny) VALUES('20170102', '01-03-2017', '01-03-2017', '98,6', '20170217', 'xxKebabmajsterxx');
INSERT INTO Test (ID_test, Datum_zaciatku, Datum_konca, Hodnotenie, ID_patch, Nickname_schvaleny) VALUES('20170214', '01-04-2017', '01-05-2017', '91,8', '20170219', null);
INSERT INTO Test (ID_test, Datum_zaciatku, Datum_konca, Hodnotenie, ID_patch, Nickname_schvaleny) VALUES('20170215', '02-03-2017', '05-03-2017', '100,0', '20170218', null);
INSERT INTO Test (ID_test, Datum_zaciatku, Datum_konca, Hodnotenie, ID_patch, Nickname_schvaleny) VALUES('20170216', '07-01-2017', '07-05-2017', '100,0', '20170216', null);
INSERT INTO Test (ID_test, Datum_zaciatku, Datum_konca, Hodnotenie, ID_patch, Nickname_schvaleny) VALUES('20170217', '13-03-2017', '13-03-2018', '9,6', '20170212', 'Ahojakosamas');

/*2 JOIN - Mena programatorov s rankom vyssim ako 5, ktori schvalili testy s hodnotenim nizsim ako 90*/
SELECT Test.Nickname_schvaleny FROM Test JOIN Programator ON Test.Nickname_schvaleny = Programator.Nickname WHERE Test.Hodnotenie <= 90 AND Programator.Rank > 5;

/*2 JOIN + IN - Aka je zavaznost bugov ktore su zatial nepridelene*/
SELECT Zavaznost FROM BUG WHERE ID_bug IN (SELECT ID_bug FROM Ticket JOIN Charakterizuje ON Ticket.ID_ticket = Charakterizuje.ID_ticket WHERE Stav = 'New');

/*2 JOIN - Aky rank ma uzivatel ktory zodpoveda za modul cislo 2*/
SELECT Rank FROM Programator JOIN Modul ON Programator.Nickname = Modul.Nickname_zodpovedny WHERE ID_modul = 2;

/*3 JOIN - Aky patch opravil zranitelnost bugu 2190732*/
SELECT Patch.ID_patch FROM Bug JOIN Patch ON Bug.ID_patch = Patch.ID_patch JOIN Zranitelnost ON Zranitelnost.ID_vulnerablity = Bug.ID_bug WHERE ID_vulnerablity = 2190732;

/*3 JOIN + IN - Na akych bugoch pracuje dany programator*/
SELECT ID_Bug FROM Bug WHERE ID_bug IN (SELECT ID_bug FROM Ticket JOIN Charakterizuje ON Ticket.ID_ticket = Charakterizuje.ID_ticket WHERE Ticket.Nickname_prideleny = 'xxKebabmajsterxx');

/*3 JOIN + EXISTS - Ktore zranitelnosti sa testuju*/
SELECT ID_vulnerablity FROM Zranitelnost WHERE EXISTS (SELECT ID_bug FROM Test JOIN Patch ON Test.ID_patch = Patch.ID_patch JOIN Bug ON Patch.ID_patch = Bug.ID_patch);

/*GROUP BY, AGGREGATE COUNT, ORDER BY - Pocet patchov vydanych jednotlivymi uzivatelmi*/
SELECT Nickname_vydany, COUNT(ID_patch) FROM Patch GROUP BY Nickname_vydany ORDER BY COUNT(ID_patch) DESC; 

/*GROUP BY, AGGREGATE COUNT, MAX, ORDER BY - Ktore moduly maju kolko bugov*/
SELECT modul_id, bug_cnt FROM (SELECT Modul.ID_modul AS modul_id,COUNT(ID_bug) AS bug_cnt FROM Bug JOIN Modul ON Bug.ID_modul = Modul.ID_modul GROUP BY Modul.ID_modul) ORDER BY bug_cnt DESC;

/*GROUP BY, AGGREGATE COUNT - Pocet ticketov k bugom podanym pred rokom 2017*/
SELECT ID_bug, COUNT(ID_ticket) FROM Charakterizuje WHERE Datum_vyskytu <= '01/01/2017' GROUP BY Charakterizuje.ID_bug;