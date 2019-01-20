CREATE DATABASE university;
CREATE TABLE departments(
	id INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
	name VARCHAR(100) NOT NULL
);

CREATE TABLE courses(
	id VARCHAR(10) NOT NULL PRIMARY KEY,
	name VARCHAR(100) NOT NULL,
	dept_id INT NOT NULL,
	FOREIGN KEY (dept_id) REFERENCES departments(id)
);

CREATE TABLE lecturers(
	id INT NOT NULL PRIMARY KEY AUTO_INCREMENT,
	name VARCHAR(100) NOT NULL,
	dept_id INT NOT NULL,
	FOREIGN KEY (dept_id) REFERENCES departments(id)
);

CREATE TABLE modules(
	id INT NOT NULL PRIMARY KEY AUTO_INCREMENT,
	name VARCHAR(100) NOT NULL,
	lecturer_id INT NOT NULL,
	FOREIGN KEY (lecturer_id) REFERENCES lecturers(id) 
);

CREATE TABLE students(
	rollNo INT NOT NULL PRIMARY KEY,
	name VARCHAR(100) NOT NULL,
	gender VARCHAR(1) NOT NULL
);

CREATE TABLE courses_modules(
	course_id VARCHAR(10),
	mod_id INT,
	PRIMARY KEY(course_id, mod_id),
	FOREIGN KEY (course_id) REFERENCES courses(id),
	FOREIGN KEY (mod_id) REFERENCES modules(id)
);

CREATE TABLE enrol(
	course_id VARCHAR(10),
	stud_id INT,
	PRIMARY KEY(course_id, stud_id),
	FOREIGN KEY (course_id) REFERENCES courses(id),
	FOREIGN KEY (stud_id) REFERENCES students(rollNo)
);

CREATE TABLE study_mod(
	mod_id INT,
	stud_id INT,
	PRIMARY KEY(mod_id, stud_id),
	FOREIGN KEY (mod_id) REFERENCES modules(id),
	FOREIGN KEY (stud_id) REFERENCES students(rollNo)
);

CREATE TABLE tutor(
	lecturer_id INT,
	stud_id INT,
	PRIMARY KEY(lecturer_id, stud_id),
	FOREIGN KEY(lecturer_id) REFERENCES lecturers(id),
	FOREIGN KEY(stud_id) REFERENCES students(rollNo)
);

INSERT INTO students VALUES(160101072, 'sparsh', 'M');
INSERT INTO students VALUES(160101039, 'kapil', 'M');
INSERT INTO students VALUES(160101066, 'shivam', 'M');

INSERT INTO departments (name) VALUES('CSE');
INSERT INTO departments (name) VALUES('MNC');

INSERT INTO courses (id, name ,dept_id) VALUES('CS201', 'Algorithms', 1);
INSERT INTO courses (id, name ,dept_id) VALUES('CS345', 'Databases', 1);
INSERT INTO courses (id, name ,dept_id) VALUES('MA321', 'Optimization', 2);

INSERT INTO lecturers (id, name,dept_id) VALUES(1, 'Santosh Biswas',1);
INSERT INTO lecturers (id, name,dept_id) VALUES(2, 'Sukanta Pati',2);
 
INSERT INTO modules (name, lecturer_id) VALUES('Divide and Conquer', 1);
INSERT INTO modules (name, lecturer_id) VALUES('DP', 1);
INSERT INTO modules (name, lecturer_id) VALUES('LPP', 2);

INSERT INTO courses_modules VALUES('CS201',1);
INSERT INTO courses_modules VALUES('CS201',2);
INSERT INTO courses_modules VALUES('MA321',3);

INSERT INTO enrol VALUES('MA321',160101039);
INSERT INTO enrol VALUES('CS201',160101039);
INSERT INTO enrol VALUES('CS345',160101039);
INSERT INTO enrol VALUES('CS201',160101072);
INSERT INTO enrol VALUES('CS345',160101066);

INSERT INTO study_mod VALUES(1,160101066);
INSERT INTO study_mod VALUES(2,160101072);
INSERT INTO study_mod VALUES(3,160101039);


