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
  ID_modul int NOT NULL UNIQUE,
  chybovost float DEFAULT NULL,
  datum_poslednej_upravy date DEFAULT NULL,
  
  Nickname NVARCHAR2(64),
  
  PRIMARY KEY (ID_modul)
);

CREATE TABLE Bug (
  ID_bug int NOT NULL UNIQUE,
  Typ NVARCHAR2(128), /*Arithmetic, Logic, Syntax, Resource, Multi-threading, Interfacing, Performance, Not a bug*/
  Zavaznost NVARCHAR2(128), /*Low, Medium, High*/
  ID_modul int NOT NULL,
  ID_patch int, 

  PRIMARY KEY (ID_bug)
);

CREATE TABLE Ticket (
  ID_ticket int NOT NULL UNIQUE,
  Datum_podania date NOT NULL,
  Stav NVARCHAR2(128), /*New, Assigned, Open, Need Info, Closed*/
  Nickname_prideleny NVARCHAR2(64),
  Nickname_vyvoreny NVARCHAR2(64) NOT NULL,

  PRIMARY KEY(ID_ticket)
);

CREATE TABLE Patch(
  ID_patch int NOT NULL UNIQUE,
  Schvalenie int,
  Datum_vydania date,
  Datum_zavedenia date,

  Nickname NVARCHAR2(64),

  PRIMARY KEY (ID_patch)
);

CREATE TABLE Test (
  ID_test int NOT NULL UNIQUE,
  Datum_zaciatku DATE,
  Datum_konca DATE,
  Hodnotenie float,

  ID_patch int,
  Nickname NVARCHAR2(64),

  PRIMARY KEY (ID_test)
);


CREATE TABLE Uzivatel (

  Nickname NVARCHAR2(64) NOT NULL UNIQUE,
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
  Popis_vyskytu NVARCHAR2(512),
  Popis_problemu NVARCHAR2(512),

  PRIMARY KEY (ID_ticket,ID_bug)
);

CREATE TABLE Zranitelnost(
  ID_vulnerablity INT UNIQUE NOT NULL,
  Miera_nebezpecenstva NVARCHAR2(64),
  Moznost_zneuzitia NVARCHAR2(64),
  
  PRIMARY KEY (ID_vulnerablity)
);

/*ALTER TABLE Klient ADD CONSTRAINT PK_klient PRIMARY KEY (r_cislo);
ALTER TABLE Ucet ADD CONSTRAINT FK_ucet_rcislo FOREIGN KEY (r_cislo) 
   REFERENCES Klient ON DELETE CASCADE;
*/

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



INSERT INTO Modul (ID_modul, chybovost, datum_poslednej_upravy) VALUES('1', '1.72', '2016-12-30');
INSERT INTO Modul (ID_modul, chybovost, datum_poslednej_upravy) VALUES('2', '2.83', '2017-03-24');
INSERT INTO Modul (ID_modul, chybovost, datum_poslednej_upravy) VALUES('3', '1.14', '2017-02-17');
INSERT INTO Modul (ID_modul, chybovost, datum_poslednej_upravy) VALUES('4', '3.84', '2015-11-16');
INSERT INTO Modul (ID_modul, chybovost, datum_poslednej_upravy) VALUES('5', '0.12', '2014-01-14');

INSERT INTO Bug (ID_bug, ID_modul, Typ, Zavaznost) VALUES('2190732', '1', 'Logic', 'High');
INSERT INTO Bug (ID_bug, ID_modul, Typ, Zavaznost) VALUES('2190746', '2', 'Syntax', 'Medium');
INSERT INTO Bug (ID_bug, ID_modul, Typ, Zavaznost) VALUES('2190789', '3', 'Not a bug', 'Low');
INSERT INTO Bug (ID_bug, ID_modul, Typ, Zavaznost) VALUES('2190719', '4', 'Interfacing', 'Medium');
INSERT INTO Bug (ID_bug, ID_modul, Typ, Zavaznost) VALUES('2190729', '5', 'Resource', 'Low');

INSERT INTO Ticket (ID_ticket, Datum_podania, Stav) VALUES('46864', '2017-03-23', 'Open');
INSERT INTO Ticket (ID_ticket, Datum_podania, Stav) VALUES('48864', '2017-02-13', 'Assigned');
INSERT INTO Ticket (ID_ticket, Datum_podania, Stav) VALUES('41864', '2017-01-23', 'New');
INSERT INTO Ticket (ID_ticket, Datum_podania, Stav) VALUES('41865', '2017-11-02', 'Closed');
INSERT INTO Ticket (ID_ticket, Datum_podania, Stav) VALUES('41868', '2017-03-24', 'Need Info');

INSERT INTO Uzivatel (Nickname, Meno, Vek, Programovaci_jazyk, ID_modul, ID_ticket, ID_test, ID_patch) VALUES('xxKebabmajsterxx', 'Peter Jablko', '21', 'Java, C++, C#', '1', '46864', '20170102', '1354');
INSERT INTO Uzivatel (Nickname, Meno, Vek, Programovaci_jazyk, ID_modul, ID_ticket, ID_test, ID_patch) VALUES('AndreDankojeLegenda', 'Juraj Zemiak', '22', 'JavaScript, HTML, PHP', '2', '48864', '20170214', '2684');
INSERT INTO Uzivatel (Nickname, Meno, Vek, Programovaci_jazyk, ID_modul, ID_ticket, ID_test, ID_patch) VALUES('MirrorMaster12', 'Jan Penaze', '23', 'Ruby, Python', '3', '41864', '20170215', '1654');
INSERT INTO Uzivatel (Nickname, Meno, Vek, Programovaci_jazyk, ID_modul, ID_ticket, ID_test, ID_patch) VALUES('Cyborg13', 'Ondrej Vychodnar', '27', '', '4', '41865', '20170216', '7896');
INSERT INTO Uzivatel (Nickname, Meno, Vek, Programovaci_jazyk, ID_modul, ID_ticket, ID_test, ID_patch) VALUES('Knedla8', 'Michal Krivozub', '19', '', '5', '41868', '20170217', '2431');
INSERT INTO Uzivatel (Nickname, Meno, Vek, Programovaci_jazyk, ID_modul, ID_ticket, ID_test, ID_patch) VALUES('Destroyer-ultimate', 'Daniel Drevo', '33', 'C++,C', '1', '46864', '20170218', '2824');
INSERT INTO Uzivatel (Nickname, Meno, Vek, Programovaci_jazyk, ID_modul, ID_ticket, ID_test, ID_patch) VALUES('Ahojakosamas', 'Jozef Koleso', '28', 'C#, .Net', '2', '48864', '20170219', '2334');
INSERT INTO Uzivatel (Nickname, Meno, Vek, Programovaci_jazyk, ID_modul, ID_ticket, ID_test, ID_patch) VALUES('Iliketurtles', 'Adrian Karfiol', '31', 'Self, Smalltalk', '3', '41864', '20170220', '1135');
INSERT INTO Uzivatel (Nickname, Meno, Vek, Programovaci_jazyk, ID_modul, ID_ticket, ID_test, ID_patch) VALUES('Whysoserious', 'Tomas Exekutor', '30', 'Delphi, Pascal', '4', '41865', '20170221', '9846');
INSERT INTO Uzivatel (Nickname, Meno, Vek, Programovaci_jazyk, ID_modul, ID_ticket, ID_test, ID_patch) VALUES('NovaZilina', 'Silvester Akalas', '18', 'GO, D', '5', '41868', '20170222', '7896');

INSERT INTO Test (ID_test, Datum_zaciatku, Datum_konca, Hodnotenie, ID_patch) VALUES('20170102', '2017-03-01 12:02:32', '2017-03-02 19:02:06', '98.6', '1354');
INSERT INTO Test (ID_test, Datum_zaciatku, Datum_konca, Hodnotenie, ID_patch) VALUES('20170214', '2017-04-01 18:03:18', '2017-04-05 17:00:07', '91.8', '2684');
INSERT INTO Test (ID_test, Datum_zaciatku, Datum_konca, Hodnotenie, ID_patch) VALUES('20170215', '2017-03-02 15:12:17', '2017-05-03 16:01:09', '100.0', '1654');
INSERT INTO Test (ID_test, Datum_zaciatku, Datum_konca, Hodnotenie, ID_patch) VALUES('20170216', '2017-01-07 14:22:03', '2017-06-02 15:02:00', '100.0', '2824');
INSERT INTO Test (ID_test, Datum_zaciatku, Datum_konca, Hodnotenie, ID_patch) VALUES('20170217', '2017-03-13 13:02:16', '2017-07-01 14:03:01', '9.6', '2431');


INSERT INTO Patch (ID_patch, Schvalenie, Datum_vydania, Datum_zavedenia) VALUES('20170217', 'FALSE', '2017-12-03', '');
INSERT INTO Patch (ID_patch, Schvalenie, Datum_vydania, Datum_zavedenia) VALUES('20170217', 'TRUE', '2017-11-07', '2017-12-05');
INSERT INTO Patch (ID_patch, Schvalenie, Datum_vydania, Datum_zavedenia) VALUES('20170217', 'TRUE', '2017-10-06', '2017-12-06');
INSERT INTO Patch (ID_patch, Schvalenie, Datum_vydania, Datum_zavedenia) VALUES('20170217', 'TRUE', '2017-09-05', '2017-12-07');
INSERT INTO Patch (ID_patch, Schvalenie, Datum_vydania, Datum_zavedenia) VALUES('20170217', 'TRUE', '2017-08-04', '2017-12-08');


