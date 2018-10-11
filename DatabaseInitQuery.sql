CREATE TABLE teacher (
	ID INT NOT NULL AUTO_INCREMENT,
    TSurname VARCHAR(23) NOT NULL,
    PRIMARY KEY(ID)
);

CREATE TABLE subject (
	ID INT NOT NULL AUTO_INCREMENT,
    SubName VARCHAR(20) NOT NULL,
    CodTeacher INT NOT NULL,
    FOREIGN KEY (CodTeacher) REFERENCES teacher(ID),
    PRIMARY KEY(ID)
);

CREATE TABLE mark (
	ID INT NOT NULL AUTO_INCREMENT,
    Mark FLOAT NOT NULL,
    MarkDate DATE NOT NULL,
    Description VARCHAR(28),
    CodSub INT NOT NULL,
    FOREIGN KEY (CodSub) REFERENCES subject(ID),
    PRIMARY KEY(ID)
);

" insert a new subject"

SET @var = (SELECT ID FROM teacher WHERE TSurname = "Rossi" LIMIT 1);
INSERT INTO subject (SubName,CodTeacher) VALUES ("Computer Science", @var);

" SQLite version of above "

CREATE TABLE IF NOT EXISTS teacher (
    ID INTEGER PRIMARY KEY,
    TSurname TEXT NOT NULL
);

CREATE TABLE IF NOT EXISTS subject (
    ID INTEGER PRIMARY KEY,
    SubName TEXT NOT NULL,
    CodTeacher INTEGER NOT NULL,
    FOREIGN KEY (CodTeacher) REFERENCES teacher(ID)
);

CREATE TABLE IF NOT EXISTS mark (
    ID INTEGER PRIMARY KEY,
    Mark FLOAT NOT NULL,
    MarkDate DATE NOT NULL,
    Description TEXT NOT NULL,
    CodSub INTEGER NOT NULL,
    FOREIGN KEY (CodSub) REFERENCES subject(ID)
);

" SQLite Select with INNER JOIN "

SELECT m.Mark, s.SubName, m.MarkDate, m.Description, t.TSurname 
FROM mark as m 
INNER JOIN subject as s 
ON m.CodSub = s.ID 
INNER JOIN teacher as t 
ON s.CodTeacher = t.ID;
