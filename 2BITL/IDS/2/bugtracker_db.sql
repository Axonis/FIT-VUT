SET FOREIGN_KEY_CHECKS=0;


DROP TABLE IF EXISTS Bug ;
DROP TABLE IF EXISTS Modul;
DROP TABLE IF EXISTS Programator;
DROP TABLE IF EXISTS Ticket;
DROP TABLE IF EXISTS Patch;
DROP TABLE IF EXISTS Test;



SET FOREIGN_KEY_CHECKS=1;




CREATE TABLE Modul (
  ID_modul int NOT NULL,
  chybovost float DEFAULT NULL,
  datum_poslednej_upravy date DEFAULT NULL,
  PRIMARY KEY (ID_modul)
);

CREATE TABLE Bug (
  ID_bug int NOT NULL,
  ID_modul int NOT NULL,
  Typ varchar(128), /*Arithmetic, Logic, Syntax, Resource, Multi-threading, Interfacing, Performance, Not a bug*/
  Zavaznost varchar(128), /*Low, Medium, High*/


  PRIMARY KEY (ID_bug),
  FOREIGN KEY (ID_modul) REFERENCES Modul(ID_modul)
);

CREATE TABLE Ticket (
  ID_ticket int NOT NULL,
  Datum_podania date NOT NULL,
  Stav varchar(128), /*New, Assigned, Open, Need Info, Closed*/

  PRIMARY KEY(ID_ticket)
);

CREATE TABLE Patch(
  ID_patch int NOT NULL,
  Schvalenie bit,
  Datum_vydania date,
  Datum_zavedenia date,

  Nickname varchar(64),


  PRIMARY KEY (ID_patch)
);


CREATE TABLE Test (
  ID_test int NOT NULL,
  Datum_zaciatku datetime,
  Datum_konca datetime,
  Hodnotenie float,

  ID_patch int,
  Nickname varchar(64),

  PRIMARY KEY (ID_test)
);


CREATE TABLE Uzivatel (

  Nickname varchar(64) NOT NULL,
  Meno varchar(128) NOT NULL,
  Vek int,
  Programovaci_jazyk varchar(512),

  ID_modul int,
  ID_ticket int,
  ID_test int,
  ID_patch int,


  PRIMARY KEY (Nickname),
  FOREIGN KEY (ID_ticket)       REFERENCES Ticket(ID_ticket),
  FOREIGN KEY (ID_modul)        REFERENCES Modul(ID_modul),
  FOREIGN KEY (ID_test)		REFERENCES Test(ID_test),
  FOREIGN KEY (ID_patch)	REFERENCES Patch(ID_patch)
);


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


