SET FOREIGN_KEY_CHECKS=0;


DROP TABLE IF EXISTS Bug ;
DROP TABLE IF EXISTS Modul;
DROP TABLE IF EXISTS Programator;
DROP TABLE IF EXISTS Ticket;


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


CREATE TABLE Uzivatel(

  Nickname int NOT NULL,
  Meno varchar(128) NOT NULLa,
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

CREATE TABLE Test(
  ID_test int NOT NULL,
  Datum_zaciatku datetime,
  Datum_konca datetime, 
  Hodnotenie float,

  ID_patch int NOT NULL,
  ID_uzivatel,

  PRIMARY KEY (ID_test),
  FOREIGN KEY (ID_patch)	REFERENCES Patch(ID_patch),
  FOREIGN KEY (ID_uzivatel)	REFERENCES Uzivatel(ID_uzivatel)
  
);

CREATE TABLE Patch(
  ID_patch int NOT NULL,
  Schvalenie bit,
  Datum_vydania date,
  Datum_zavedenia date,

  ID_uzivatel,

  PRIMARY KEY (ID_patch)
);


