SET FOREIGN_KEY_CHECKS=0;

DROP TABLE IF EXISTS Bugs                  CASCADE ;
DROP TABLE IF EXISTS Modules                CASCADE ;
DROP TABLE IF EXISTS Uzivatel             CASCADE ;
DROP TABLE IF EXISTS Ticket               CASCADE ;
DROP TABLE IF EXISTS Patch                CASCADE ;
DROP TABLE IF EXISTS Test                 CASCADE ;
DROP TABLE IF EXISTS Charakterizuje       CASCADE ;
DROP TABLE IF EXISTS Zranitelnost         CASCADE ;
DROP TABLE IF EXISTS Programator          CASCADE ;
DROP TABLE IF EXISTS Bezny_uzivatel       CASCADE ;
DROP TABLE IF EXISTS Programovaci_jazyk   CASCADE ;
DROP TABLE IF EXISTS Ovlada               CASCADE ;

SET FOREIGN_KEY_CHECKS=1;


CREATE TABLE Modules (
  ID_modul int NOT NULL,
  chybovost float DEFAULT NULL,
  datum_poslednej_upravy date DEFAULT NULL,
  Nazov_jazyka VARCHAR(64),
  
  Nickname_zodpovedny VARCHAR(64),
  
  PRIMARY KEY (ID_modul)
);

CREATE TABLE Bugs (
  ID_bug int NOT NULL AUTO_INCREMENT,
  Typ VARCHAR(128), /*Arithmetic, Logic, Syntax, Resource, Multi-threading, Interfacing, Performance, Not a bug*/
  Zavaznost VARCHAR(128), /*Low, Medium, High*/
  ID_modul int NOT NULL,
  ID_patch int, 

  PRIMARY KEY (ID_bug)
);

CREATE TABLE Ticket (
  ID_ticket int NOT NULL AUTO_INCREMENT,
  Datum_podania date NOT NULL,
  Stav VARCHAR(128), /*New, Assigned, Open, Need Info, Closed*/
  Nickname_prideleny VARCHAR(64) NULL,
  Nickname_vyvoreny VARCHAR(64) NOT NULL,

  PRIMARY KEY(ID_ticket)
);

CREATE TABLE Patch(
  ID_patch int NOT NULL,
  Schvalenie int,
  Datum_vydania date,
  Datum_zavedenia date,

  Nickname_vydany VARCHAR(64),

  PRIMARY KEY (ID_patch)
);

CREATE TABLE Test (
  ID_test int NOT NULL,
  Datum_zaciatku DATE,
  Datum_konca DATE,
  Hodnotenie float,

  ID_patch int,
  Nickname_schvaleny VARCHAR(64),

  PRIMARY KEY (ID_test)
);


CREATE TABLE Uzivatel (

  Nickname VARCHAR(64) NOT NULL,
  Meno VARCHAR(128) NOT NULL,
  Vek int,

  PRIMARY KEY (Nickname)
);

CREATE TABLE Bezny_uzivatel (
  Nickname VARCHAR(64) NOT NULL REFERENCES Uzivatel(Nickname),
  Odmena INT,
  Karma INT,
  
  PRIMARY KEY (Nickname)
);

CREATE TABLE Programator (
  Nickname VARCHAR(64) NOT NULL REFERENCES Uzivatel(Nickname),
  Produkt VARCHAR(64),
  Rank INT,
  
  PRIMARY KEY (Nickname) 
);

CREATE TABLE Programovaci_jazyk (
  Nazov_jazyka VARCHAR(64) NOT NULL,
  
  PRIMARY KEY(Nazov_jazyka)
);

CREATE TABLE Ovlada (
  Nickname VARCHAR(64) NOT NULL,
  Nazov_jazyka VARCHAR(64) NOT NULL,
  
  Skusenost VARCHAR(64),/*Beginner, Advanced, Expert*/
  
  PRIMARY KEY (Nickname,Nazov_jazyka)
);


CREATE TABLE Charakterizuje (
  ID_ticket int NOT NULL,
  ID_bug int NOT NULL,

  Datum_vyskytu DATE,
  Frekvencia_vyskytu INT,
  Popis_vyskytu VARCHAR(1024),
  Popis_problemu VARCHAR(1024),

  PRIMARY KEY (ID_ticket,ID_bug)
);

CREATE TABLE Zranitelnost(
  ID_vulnerablity INT NOT NULL REFERENCES Bug(ID_bug),
  Miera_nebezpecenstva VARCHAR(64), /*Low, Medium, High, Critical*/
  Moznost_zneuzitia VARCHAR(64),
  
  PRIMARY KEY (ID_vulnerablity)   
);


ALTER TABLE Modul ADD FOREIGN KEY (Nickname_zodpovedny) REFERENCES Uzivatel(Nickname);
ALTER TABLE Bug ADD FOREIGN KEY (ID_modul) REFERENCES Modul(ID_modul);
ALTER TABLE Bug ADD FOREIGN KEY (ID_patch) REFERENCES Patch(ID_patch);
ALTER TABLE Ticket ADD FOREIGN KEY (Nickname_prideleny) REFERENCES Uzivatel(Nickname);
ALTER TABLE Ticket ADD FOREIGN KEY (Nickname_vyvoreny) REFERENCES Uzivatel(Nickname);

ALTER TABLE Patch ADD FOREIGN KEY (Nickname_vydany) REFERENCES Uzivatel(Nickname);
ALTER TABLE Test ADD FOREIGN KEY (ID_patch) REFERENCES Patch(ID_patch);
ALTER TABLE Test ADD FOREIGN KEY (Nickname_schvaleny) REFERENCES Uzivatel(Nickname);

ALTER TABLE Charakterizuje ADD FOREIGN KEY (ID_ticket) REFERENCES Ticket(ID_ticket);
ALTER TABLE Charakterizuje ADD FOREIGN KEY (ID_bug) REFERENCES Bug(ID_bug);

ALTER TABLE Ovlada ADD FOREIGN KEY (Nickname) REFERENCES Uzivatel(Nickname);
ALTER TABLE Ovlada ADD FOREIGN KEY (Nazov_jazyka) REFERENCES Programovaci_jazyk(Nazov_jazyka);

ALTER TABLE Modul ADD FOREIGN KEY (Nazov_jazyka) REFERENCES Programovaci_jazyk(Nazov_jazyka);

ALTER TABLE Modul ADD CONSTRAINT chk_chybovost CHECK (chybovost >= 0 AND chybovost <= 100);
ALTER TABLE Test ADD CONSTRAINT chk_hodnotenie CHECK (Hodnotenie >= 0 AND Hodnotenie <= 100);


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

INSERT INTO Modul (ID_modul, chybovost, datum_poslednej_upravy, Nickname_zodpovedny, Nazov_jazyka) VALUES('1', '1.25', '2016-12-21', 'NovaZilina', 'Java');
INSERT INTO Modul (ID_modul, chybovost, datum_poslednej_upravy, Nickname_zodpovedny, Nazov_jazyka) VALUES('2', '2.83', '2017-03-24', 'MirrorMaster12', 'C');
INSERT INTO Modul (ID_modul, chybovost, datum_poslednej_upravy, Nickname_zodpovedny, Nazov_jazyka) VALUES('3', '1.14', '2017-02-22', 'xxKebabmajsterxx', 'GO');
INSERT INTO Modul (ID_modul, chybovost, datum_poslednej_upravy, Nickname_zodpovedny, Nazov_jazyka) VALUES('4', '3.84', '2015-11-16', 'AndreDankojeLegenda', 'Python');
INSERT INTO Modul (ID_modul, chybovost, datum_poslednej_upravy, Nickname_zodpovedny, Nazov_jazyka) VALUES('5', '0.12', '2016-01-14', 'xxKebabmajsterxx', 'C++');
INSERT INTO Modul (ID_modul, chybovost, datum_poslednej_upravy, Nickname_zodpovedny, Nazov_jazyka) VALUES('6', '0.00', '2016-06-14', 'NovaZilina', 'D');

INSERT INTO Patch (ID_patch, Schvalenie, Datum_vydania, Datum_zavedenia, Nickname_vydany) VALUES('20170217', '0', '2017-03-12', '0000-00-00', 'xxKebabmajsterxx');
INSERT INTO Patch (ID_patch, Schvalenie, Datum_vydania, Datum_zavedenia, Nickname_vydany) VALUES('20170219', '1', '2017-07-11', '2017-08-12', 'Ahojakosamas');
INSERT INTO Patch (ID_patch, Schvalenie, Datum_vydania, Datum_zavedenia, Nickname_vydany) VALUES('20170218', '1', '2017-06-10', '2017-06-12', 'Cyborg13');
INSERT INTO Patch (ID_patch, Schvalenie, Datum_vydania, Datum_zavedenia, Nickname_vydany) VALUES('20170216', '1', '2017-05-09', '2017-07-12', 'Knedla8');
INSERT INTO Patch (ID_patch, Schvalenie, Datum_vydania, Datum_zavedenia, Nickname_vydany) VALUES('20170212', '1', '2017-04-08', '2017-08-12', 'Knedla8');
INSERT INTO Patch (ID_patch, Schvalenie, Datum_vydania, Datum_zavedenia, Nickname_vydany) VALUES('20170315', '1', '2017-03-15', '2017-04-03', 'xxKebabmajsterxx');



INSERT INTO Bugs (ID_modul, Typ, Zavaznost, ID_patch) VALUES('1', 'Logic', 'High', '20170219');
INSERT INTO Bugs (ID_modul, Typ, Zavaznost, ID_patch) VALUES('2', 'Syntax', 'Medium', '20170216');
INSERT INTO Bugs (ID_modul, Typ, Zavaznost, ID_patch) VALUES('3', 'Not a bug', 'Low', '20170218');
INSERT INTO Bugs (ID_modul, Typ, Zavaznost) VALUES('4', 'Interfacing', 'Medium');
INSERT INTO Bugs (ID_modul, Typ, Zavaznost, ID_patch) VALUES('5', 'Resource', 'Low', '20170315');
INSERT INTO Bugs (ID_modul, Typ, Zavaznost) VALUES('1', 'Logic', 'High');
INSERT INTO Bugs (ID_modul, Typ, Zavaznost, ID_patch) VALUES('2', 'Syntax', 'Medium', '20170217');
INSERT INTO Bugs (ID_modul, Typ, Zavaznost, ID_patch) VALUES('3', 'Logic', 'High', '20170216');
INSERT INTO Bugs (ID_modul, Typ, Zavaznost) VALUES('1', 'Resource', 'High');

INSERT INTO Zranitelnost (ID_vulnerablity, Miera_nebezpecenstva, Moznost_zneuzitia) VALUES('6', 'High', 'Modul');
INSERT INTO Zranitelnost (ID_vulnerablity, Miera_nebezpecenstva, Moznost_zneuzitia) VALUES('8', 'Critical', 'System');

INSERT INTO Tickets (Stav, name_prideleny, name_vyvoreny) VALUES('Open', 'xxKebabmajsterxx', 'Cyborg13');
INSERT INTO Tickets (Stav, name_prideleny, name_vyvoreny) VALUES('Assigned', 'MirrorMaster12', 'Knedla8');
INSERT INTO Tickets (Stav, name_prideleny, name_vyvoreny) VALUES('New', null, 'Cyborg13');
INSERT INTO Tickets (Stav, name_prideleny, name_vyvoreny) VALUES('Closed', 'Ahojakosamas', 'Knedla8');
INSERT INTO Tickets (Stav, name_prideleny, name_vyvoreny) VALUES('Need Info', 'Destroyer-ultimate', 'Cyborg13');

INSERT INTO Charakterizuje (ID_ticket, ID_bug, Datum_vyskytu, Frekvencia_vyskytu, Popis_vyskytu, Popis_problemu) VALUES ('1', '8', '2016-03-25', '10', '1.Install System, 2.Run cmd command', 'Kernel Panic');
INSERT INTO Charakterizuje (ID_ticket, ID_bug, Datum_vyskytu, Frekvencia_vyskytu, Popis_vyskytu, Popis_problemu) VALUES ('2', '7', '2015-03-13', '7', '1.Install System, 2.Run cmd command', 'Kernel Panic');
INSERT INTO Charakterizuje (ID_ticket, ID_bug, Datum_vyskytu, Frekvencia_vyskytu, Popis_vyskytu, Popis_problemu) VALUES ('2', '3', '2014-01-14', '10', '1.Install System, 2.Run cmd command', 'Kernel Panic');
INSERT INTO Charakterizuje (ID_ticket, ID_bug, Datum_vyskytu, Frekvencia_vyskytu, Popis_vyskytu, Popis_problemu) VALUES ('3', '4', '2017-04-01', '7', '1.Install System, 2.Run cmd command', 'Kernel Panic');
INSERT INTO Charakterizuje (ID_ticket, ID_bug, Datum_vyskytu, Frekvencia_vyskytu, Popis_vyskytu, Popis_problemu) VALUES ('1', '5', '2017-05-06', '8', '1.Install System, 2.Run cmd command', 'Kernel Panic');
INSERT INTO Charakterizuje (ID_ticket, ID_bug, Datum_vyskytu, Frekvencia_vyskytu, Popis_vyskytu, Popis_problemu) VALUES ('2', '6', '2017-03-07', '9', '1.Install System, 2.Run cmd command', 'Kernel Panic');
INSERT INTO Charakterizuje (ID_ticket, ID_bug, Datum_vyskytu, Frekvencia_vyskytu, Popis_vyskytu, Popis_problemu) VALUES ('3', '9', '2014-03-08', '1', '1.Install System, 2.Run cmd command', 'Kernel Panic');

INSERT INTO Test (ID_test, Datum_zaciatku, Datum_konca, Hodnotenie, ID_patch, Nickname_schvaleny) VALUES('20170102', '2014-03-01', '2017-03-01', '98.6', '20170217', 'xxKebabmajsterxx');
INSERT INTO Test (ID_test, Datum_zaciatku, Datum_konca, Hodnotenie, ID_patch, Nickname_schvaleny) VALUES('20170214', '2017-04-01', '2017-04-02', '91.8', '20170219', null);
INSERT INTO Test (ID_test, Datum_zaciatku, Datum_konca, Hodnotenie, ID_patch, Nickname_schvaleny) VALUES('20170255', '2017-04-01', null, '0.0', '20170219', null);
UPDATE Test SET Datum_konca = '2017-04-03' WHERE ID_test = 20170255;
INSERT INTO Test (ID_test, Datum_zaciatku, Datum_konca, Hodnotenie, ID_patch, Nickname_schvaleny) VALUES('20170215', '2017-03-02', '2017-03-03', '100.0', '20170218', null);
INSERT INTO Test (ID_test, Datum_zaciatku, Datum_konca, Hodnotenie, ID_patch, Nickname_schvaleny) VALUES('20170216', '2017-01-07', '2017-05-07', '100.0', '20170216', null);
INSERT INTO Test (ID_test, Datum_zaciatku, Datum_konca, Hodnotenie, ID_patch, Nickname_schvaleny) VALUES('20170217', '2017-03-13', '2018-03-13', '9.6', '20170212', 'Ahojakosamas');

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
