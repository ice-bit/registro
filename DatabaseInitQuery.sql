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


SET @var = (SELECT ID FROM teacher WHERE TSurname = "Rossi" LIMIT 1);
INSERT INTO subject (SubName,CodTeacher) VALUES ("Computer Science", @var);