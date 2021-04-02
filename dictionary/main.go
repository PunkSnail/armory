package main

import (
	"flag"
	"fmt"
	"os/user"
)

// redundant backup:
// "E0F0D336AF47D3797C68372A869BDBC5", "AA6C7429C3884C9E766C51187BD1D86F"

const DICT_KEY string = "341DEFE6E5CA504E62A567082590D0BD"
const DICT_URL string = "http://dict-co.iciba.com/api/dictionary.php"

var g_word = flag.String("s", "", "Specify the word to be queried")

func main() {

	if false == parse_input_args() {
		return
	}
	var db_data, db_path string

	if user, err := user.Current(); nil == err {
		db_path = user.HomeDir + "/.dictionary.sqlite"
	} else {
		fmt.Printf("%v\n", err)
		return
	}
	db, err := db_open(db_path)
	if nil == err {
		db_data = db_get_word(db, *g_word)
	}
	if 0 == len(db_data) {
		url_prefix := DICT_URL + "?key=" + DICT_KEY + "&w="

		if data, err := online_get_word(url_prefix, *g_word); nil == err {
			// save data to database
			db_set_word(db, *g_word, string(data))

			show_get_result(data)
		} else {
			fmt.Printf("%v\n", err)
		}
	} else {
		show_get_result([]byte(db_data))
	}
}
