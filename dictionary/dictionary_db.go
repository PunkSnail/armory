package main

import (
	"database/sql"
	"fmt"

	_ "github.com/mattn/go-sqlite3"
)

const SQL_TABLE string = `
	CREATE TABLE IF NOT EXISTS dictionary(
		uid INTEGER PRIMARY KEY AUTOINCREMENT,
		word TEXT NULL,
		content TEXT NULL);`

func db_open(sqlite_path string) (*sql.DB, error) {

	db, err := sql.Open("sqlite3", sqlite_path)

	if err == nil {
		if _, err = db.Exec(SQL_TABLE); err != nil {
			return nil, err
		}
	}
	return db, err
}

func db_set_word(db *sql.DB, key string, value string) error {

	if nil == db {
		return fmt.Errorf("invalid sql.DB")
	}
	insert_cmd := fmt.Sprintf(
		"INSERT INTO dictionary(word, content) values(?,?)")
	stmt, err := db.Prepare(insert_cmd)

	if err == nil {
		defer stmt.Close()
		_, err = stmt.Exec(key, value)
	}
	return err
}

func db_get_word(db *sql.DB, word string) string {

	var uid int
	var conent string

	if nil == db {
		return conent
	}
	query_cmd := fmt.Sprintf(
		"SELECT * FROM dictionary WHERE word = '%s'", word)

	if rows, err := db.Query(query_cmd); err == nil {
		for rows.Next() {
			rows.Scan(&uid, &word, &conent)
		}
		rows.Close()
	}
	return conent
}
