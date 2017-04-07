DROP TABLE Bug      CASCADE CONSTRAINTS;
DROP TABLE Modul    CASCADE CONSTRAINTS;
DROP TABLE Uzivatel CASCADE CONSTRAINTS;
DROP TABLE Ticket   CASCADE CONSTRAINTS;
DROP TABLE Patch    CASCADE CONSTRAINTS;
DROP TABLE Test     CASCADE CONSTRAINTS;
DROP TABLE Charakterizuje   CASCADE CONSTRAINTS;
DROP TABLE Zranitelnost     CASCADE CONSTRAINTS;
DROP TABLE Programator      CASCADE CONSTRAINTS;
DROP TABLE Bezny_uzivatel   CASCADE CONSTRAINTS;




CREATE TABLE Modul (
  ID_modul int NOT NULL,
  chybovost float DEFAULT NULL,
  datum_poslednej_upravy date DEFAULT NULL,
  
  Nickname NVARCHAR2(64),
  
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

  Nickname NVARCHAR2(64),

  PRIMARY KEY (ID_patch)
);

CREATE TABLE Test (
  ID_test int NOT NULL,
  Datum_zaciatku DATE,
  Datum_konca DATE,
  Hodnotenie float,

  ID_patch int,
  Nickname NVARCHAR2(64),

  PRIMARY KEY (ID_test)
);


CREATE TABLE Uzivatel (

  Nickname NVARCHAR2(64) NOT NULL,
  Meno NVARCHAR2(128) NOT NULL,
  Vek int,
  Programovaci_jazyk NVARCHAR2(512),

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

ALTER TABLE Modul ADD FOREIGN KEY (Nickname) REFERENCES Uzivatel;
ALTER TABLE Bug ADD FOREIGN KEY (ID_modul) REFERENCES Modul;
ALTER TABLE Bug ADD FOREIGN KEY (ID_patch) REFERENCES Patch;
ALTER TABLE Ticket ADD FOREIGN KEY (Nickname_prideleny) REFERENCES Uzivatel(Nickname);
ALTER TABLE Ticket ADD FOREIGN KEY (Nickname_vyvoreny) REFERENCES Uzivatel(Nickname);

ALTER TABLE Patch ADD FOREIGN KEY (Nickname) REFERENCES Uzivatel(Nickname);
ALTER TABLE Test ADD FOREIGN KEY (ID_patch) REFERENCES Patch(ID_patch);
ALTER TABLE Test ADD FOREIGN KEY (Nickname) REFERENCES Uzivatel(Nickname);

ALTER TABLE Charakterizuje ADD FOREIGN KEY (ID_ticket) REFERENCES Ticket(ID_ticket);
ALTER TABLE Charakterizuje ADD FOREIGN KEY (ID_bug) REFERENCES Bug(ID_bug);

ALTER TABLE Modul ADD CONSTRAINTS chk_chybovost CHECK (chybovost >= 0 AND chybovost <= 100);
ALTER TABLE Test ADD CONSTRAINTS chk_hodnotenie CHECK (Hodnotenie >= 0 AND Hodnotenie <= 100);


INSERT INTO Uzivatel (Nickname, Meno, Vek, Programovaci_jazyk) VALUES('xxKebabmajsterxx', 'Peter Jablko', '21', 'Java, C++, C#');
INSERT INTO Uzivatel (Nickname, Meno, Vek, Programovaci_jazyk) VALUES('AndreDankojeLegenda', 'Juraj Zemiak', '22', 'JavaScript, HTML, PHP');
INSERT INTO Uzivatel (Nickname, Meno, Vek, Programovaci_jazyk) VALUES('MirrorMaster12', 'Jan Penaze', '23', 'Ruby, Python');
INSERT INTO Uzivatel (Nickname, Meno, Vek, Programovaci_jazyk) VALUES('Cyborg13', 'Ondrej Vychodnar', '27', '');
INSERT INTO Uzivatel (Nickname, Meno, Vek, Programovaci_jazyk) VALUES('Knedla8', 'Michal Krivozub', '19', '');
INSERT INTO Uzivatel (Nickname, Meno, Vek, Programovaci_jazyk) VALUES('Destroyer-ultimate', 'Daniel Drevo', '33', 'C++,C');
INSERT INTO Uzivatel (Nickname, Meno, Vek, Programovaci_jazyk) VALUES('Ahojakosamas', 'Jozef Koleso', '28', 'C#, .Net');
INSERT INTO Uzivatel (Nickname, Meno, Vek, Programovaci_jazyk) VALUES('Iliketurtles', 'Adrian Karfiol', '31', 'Self, Smalltalk');
INSERT INTO Uzivatel (Nickname, Meno, Vek, Programovaci_jazyk) VALUES('Whysoserious', 'Tomas Exekutor', '30', 'Delphi, Pascal');
INSERT INTO Uzivatel (Nickname, Meno, Vek, Programovaci_jazyk) VALUES('NovaZilina', 'Silvester Akalas', '18', 'GO, D');

INSERT INTO Programator (Nickname, Produkt, Rank) VALUES('xxKebabmajsterxx', 'OpenStack', '8');
INSERT INTO Programator (Nickname, Produkt, Rank) VALUES('AndreDankojeLegenda', 'OpenStack', '6');
INSERT INTO Programator (Nickname, Produkt, Rank) VALUES('MirrorMaster12', 'Kernel', '2');
INSERT INTO Programator (Nickname, Produkt, Rank) VALUES('Ahojakosamas', 'Kernel', '9');
INSERT INTO Programator (Nickname, Produkt, Rank) VALUES('NovaZilina', 'Struska', '10');

INSERT INTO Bezny_uzivatel (Nickname, Odmena, Karma) VALUES('Cyborg13', null, null);
INSERT INTO Bezny_uzivatel (Nickname, Odmena, Karma) VALUES('Destroyer-ultimate', '10000', '1000');
INSERT INTO Bezny_uzivatel (Nickname, Odmena, Karma) VALUES('Whysoserious', '56464', '56');

INSERT INTO Modul (ID_modul, chybovost, datum_poslednej_upravy, Nickname) VALUES('1', '1,25', '21-12-2016', 'NovaZilina');
INSERT INTO Modul (ID_modul, chybovost, datum_poslednej_upravy, Nickname) VALUES('2', '2,83', '24-03-2017', 'MirrorMaster12');
INSERT INTO Modul (ID_modul, chybovost, datum_poslednej_upravy, Nickname) VALUES('3', '1,14', '17-02-2017', 'xxKebabmajsterxx');
INSERT INTO Modul (ID_modul, chybovost, datum_poslednej_upravy, Nickname) VALUES('4', '3,84', '16-11-2015', 'AndreDankojeLegenda');
INSERT INTO Modul (ID_modul, chybovost, datum_poslednej_upravy, Nickname) VALUES('5', '0,12', '14-01-2016', 'xxKebabmajsterxx');

INSERT INTO Patch (ID_patch, Schvalenie, Datum_vydania, Datum_zavedenia) VALUES('20170217', '0', '03-12-2017', '');
INSERT INTO Patch (ID_patch, Schvalenie, Datum_vydania, Datum_zavedenia) VALUES('20170219', '1', '07-11-2017', '05-12-2017');
INSERT INTO Patch (ID_patch, Schvalenie, Datum_vydania, Datum_zavedenia) VALUES('20170218', '1', '06-10-2017', '06-12-2017');
INSERT INTO Patch (ID_patch, Schvalenie, Datum_vydania, Datum_zavedenia) VALUES('20170216', '1', '05-09-2017', '07-12-2017');
INSERT INTO Patch (ID_patch, Schvalenie, Datum_vydania, Datum_zavedenia) VALUES('20170212', '1', '04-08-2017', '08-12-2017');

INSERT INTO Bug (ID_bug, ID_modul, Typ, Zavaznost, ID_patch) VALUES('2190732', '1', 'Logic', 'High', '20170219');
INSERT INTO Bug (ID_bug, ID_modul, Typ, Zavaznost, ID_patch) VALUES('2190746', '2', 'Syntax', 'Medium', '20170216');
INSERT INTO Bug (ID_bug, ID_modul, Typ, Zavaznost) VALUES('2190789', '3', 'Not a bug', 'Low');
INSERT INTO Bug (ID_bug, ID_modul, Typ, Zavaznost) VALUES('2190719', '4', 'Interfacing', 'Medium');
INSERT INTO Bug (ID_bug, ID_modul, Typ, Zavaznost) VALUES('2190729', '5', 'Resource', 'Low');

INSERT INTO Zranitelnost (ID_vulnerablity, Miera_nebezpecenstva, Moznost_zneuzitia) VALUES('2190732', 'High', 'Modul');
INSERT INTO Zranitelnost (ID_vulnerablity, Miera_nebezpecenstva, Moznost_zneuzitia) VALUES('2190746', 'Critical', 'System');

INSERT INTO Ticket (ID_ticket, Datum_podania, Stav, Nickname_prideleny, Nickname_vyvoreny) VALUES('46864', '23-03-2017', 'Open', 'xxKebabmajsterxx', 'Cyborg13');
INSERT INTO Ticket (ID_ticket, Datum_podania, Stav, Nickname_prideleny, Nickname_vyvoreny) VALUES('48864', '13-02-2017', 'Assigned', 'MirrorMaster12', 'Knedla8');
INSERT INTO Ticket (ID_ticket, Datum_podania, Stav, Nickname_prideleny, Nickname_vyvoreny) VALUES('41864', '23-01-2017', 'New', 'Whysoserious', 'Cyborg13');
INSERT INTO Ticket (ID_ticket, Datum_podania, Stav, Nickname_prideleny, Nickname_vyvoreny) VALUES('41865', '02-11-2017', 'Closed', 'Ahojakosamas', 'Knedla8');
INSERT INTO Ticket (ID_ticket, Datum_podania, Stav, Nickname_prideleny, Nickname_vyvoreny) VALUES('41868', '24-03-2017', 'Need Info', 'Destroyer-ultimate', 'Cyborg13');

INSERT INTO Charakterizuje (ID_ticket, ID_bug, Datum_vyskytu, Frekvencia_vyskytu, Popis_vyskytu, Popis_problemu) VALUES ('46864', '2190732', '01-03-2017', '10', '1.Install System, 2.Run cmd command', 'Kernel Panic');
INSERT INTO Charakterizuje (ID_ticket, ID_bug, Datum_vyskytu, Frekvencia_vyskytu, Popis_vyskytu, Popis_problemu) VALUES ('48864', '2190732', '01-03-2017', '7', '1.Install System, 2.Run cmd command', 'Kernel Panic');
INSERT INTO Charakterizuje (ID_ticket, ID_bug, Datum_vyskytu, Frekvencia_vyskytu, Popis_vyskytu, Popis_problemu) VALUES ('48864', '2190746', '01-03-2017', '10', '1.Install System, 2.Run cmd command', 'Kernel Panic');
INSERT INTO Charakterizuje (ID_ticket, ID_bug, Datum_vyskytu, Frekvencia_vyskytu, Popis_vyskytu, Popis_problemu) VALUES ('41864', '2190746', '01-03-2017', '7', '1.Install System, 2.Run cmd command', 'Kernel Panic');

INSERT INTO Test (ID_test, Datum_zaciatku, Datum_konca, Hodnotenie, ID_patch, Nickname) VALUES('20170102', '01-03-2017', '01-03-2017', '98,6', '20170217', 'xxKebabmajsterxx');
INSERT INTO Test (ID_test, Datum_zaciatku, Datum_konca, Hodnotenie, ID_patch, Nickname) VALUES('20170214', '01-04-2017', '01-05-2017', '91,8', '20170219', null);
INSERT INTO Test (ID_test, Datum_zaciatku, Datum_konca, Hodnotenie, ID_patch, Nickname) VALUES('20170215', '02-03-2017', '05-03-2017', '100,0', '20170218', null);
INSERT INTO Test (ID_test, Datum_zaciatku, Datum_konca, Hodnotenie, ID_patch, Nickname) VALUES('20170216', '07-01-2017', '07-05-2017', '100,0', '20170216', null);
INSERT INTO Test (ID_test, Datum_zaciatku, Datum_konca, Hodnotenie, ID_patch, Nickname) VALUES('20170217', '13-03-2017', '13-03-2018', '9,6', '20170212', 'Ahojakosamas');

