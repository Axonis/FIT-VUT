SET FOREIGN_KEY_CHECKS=0;


DROP TABLE IF EXISTS Bug ;
DROP TABLE IF EXISTS Modul;
DROP TABLE IF EXISTS Programator;
DROP TABLE IF EXISTS Ticket;


SET FOREIGN_KEY_CHECKS=1;




CREATE TABLE Modul (
  ID_modul int NOT NULL,
  chybovost bigint DEFAULT NULL,
  datum_poslednej_upravy datetime DEFAULT NULL,
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


CREATE TABLE Programator(
  ID_employee int NOT NULL,
  Rank int,

  ID_modul int,
  ID_ticket int,
  ID_test int,



  PRIMARY KEY (ID_employee),
  FOREIGN KEY (ID_ticket)       REFERENCES Ticket(ID_ticket),
  FOREIGN KEY (ID_modul)        REFERENCES Modul(ID_modul)
);


INSERT INTO Modul (ID_modul, chybovost, datum_poslednej_upravy) VALUES('1', '1.72', '2016-12-30');
INSERT INTO Modul (ID_modul, chybovost, datum_poslednej_upravy) VALUES('2', '2.83', '2017-03-24');
INSERT INTO Modul (ID_modul, chybovost, datum_poslednej_upravy) VALUES('3', '1.14', '2017-02-17');

INSERT INTO Bug (ID_bug, ID_modul, Typ, Zavaznost) VALUES('2190732', '1', 'Logic', 'High');
INSERT INTO Bug (ID_bug, ID_modul, Typ, Zavaznost) VALUES('2190746', '2', 'Syntax', 'Medium');
INSERT INTO Bug (ID_bug, ID_modul, Typ, Zavaznost) VALUES('2190789', '3', 'Interfacing', 'Low');

INSERT INTO Ticket (ID_ticket, Datum_podania, Stav) VALUES('46864', '2017-03-23', 'Open');
INSERT INTO Ticket (ID_ticket, Datum_podania, Stav) VALUES('48864', '2017-02-13', 'Assigned');
INSERT INTO Ticket (ID_ticket, Datum_podania, Stav) VALUES('41864', '2017-01-23', 'New');




