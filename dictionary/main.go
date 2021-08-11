package main

import (
	"flag"
	"fmt"
	"os"
	"os/user"
	"strings"
)

// redundant backup:
// "E0F0D336AF47D3797C68372A869BDBC5", "AA6C7429C3884C9E766C51187BD1D86F"

const DICT_KEY string = "341DEFE6E5CA504E62A567082590D0BD"
const DICT_URL string = "http://dict-co.iciba.com/api/dictionary.php"

var g_word = flag.String("w", "", "word 指定要查询的单词")
var g_external = flag.Bool("e", false, "external 显示外部读音 URL")
var g_sound = flag.Bool("s", false, "sound 在线获取读音 (need Sox)")
var g_online = flag.Bool("o", false, "online 获取线上非本地结果")

func parse_input_args() bool {

	if 1 == len(os.Args) {
		fmt.Printf("missing args, try: %s --help\n", os.Args[0])
		return false
	}
	if flag.Parse(); len(*g_word) < 1 {
		fmt.Println("missing single word")
		return false
	}
	return true
}

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

	if false == *g_online && nil == err {
		db_data = db_get_word(db, *g_word)
	}
	if 0 == len(db_data) {
		url_prefix := DICT_URL + "?key=" + DICT_KEY + "&w="

		if data, err := online_get_word(url_prefix, *g_word); nil == err {

			if strings.Contains(string(data), "sent") {
				// example sentence exists, save data to the database
				db_set_word(db, *g_word, string(data))
			}
			presenting_result(data, *g_external, *g_sound)
		} else {
			fmt.Printf("%v\n", err)
		}
	} else {
		presenting_result([]byte(db_data), *g_external, *g_sound)
	}
}
