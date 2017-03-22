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
  Typ varchar(128),
  Zavaznost varchar(128),


  PRIMARY KEY (ID_bug),
  FOREIGN KEY (ID_modul) REFERENCES Modul(ID_modul)
);

CREATE TABLE Ticket (
  ID_ticket int NOT NULL,
  Datum_podania date NOT NULL,
  Stav varchar(128),

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

