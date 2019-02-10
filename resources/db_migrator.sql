/* Script used to migrate to  
 * the newer version of Registro
 */
PRAGMA foreign_keys = off;

BEGIN TRANSACTION;

ALTER TABLE subject RENAME TO subject_old;
ALTER TABLE mark RENAME TO mark_old;
ALTER TABLE teacher RENAME TO teachers;

CREATE TABLE subjects (
    ID INTEGER PRIMARY KEY,
    SubName TEXT NOT NULL,
    CodTeacher INTEGER NOT NULL,
    CONSTRAINT fk_teachers 
        FOREIGN KEY (CodTeacher) REFERENCES teachers(ID) ON DELETE CASCADE
);

CREATE TABLE marks (
    ID INTEGER PRIMARY KEY,
    Mark FLOAT NOT NULL,
    MarkDate DATE NOT NULL,
    Description TEXT NOT NULL,
    CodSub INTEGER NOT NULL,
    CONSTRAINT fk_subjects 
        FOREIGN KEY (CodSub) REFERENCES subjects(ID) ON DELETE CASCADE
);

INSERT INTO subjects SELECT * FROM subject_old;
INSERT INTO marks SELECT * FROM mark_old;

DROP TABLE mark_old;
DROP TABLE subject_old;

COMMIT;

PRAGMA foreign_keys = on;