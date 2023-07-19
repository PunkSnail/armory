package main

import (
	"database/sql"
	"flag"
	"fmt"
	"os"
	"os/user"
)

// redundant backup:
// "E0F0D336AF47D3797C68372A869BDBC5", "AA6C7429C3884C9E766C51187BD1D86F"

const DICT_KEY string = "341DEFE6E5CA504E62A567082590D0BD"
const DICT_URL string = "http://dict-co.iciba.com/api/dictionary.php"

var g_word = flag.String("w", "", "word 指定要查询的单词")
var g_external = flag.Bool("e", false, "external 显示外部读音 URL")
var g_sound = flag.Bool("s", false, "sound 在线获取读音 (need Sox)")
var g_database = flag.Bool("d", false, "database 优先查询本地数据库")
var g_interactive = flag.Bool("i", false, "interactive 进入交互词典")

func parse_input_args(p_conf *Configure) bool {

	if 1 == len(os.Args) {
		fmt.Printf("missing args, try: %s --help\n", os.Args[0])
		return false
	}
	if flag.Parse(); len(*g_word) < 1 && false == *g_interactive {
		fmt.Println("missing single word")
		return false
	}
	p_conf.External = *g_external
	p_conf.Sound = *g_sound
	p_conf.Database = *g_database
	p_conf.Interactive = *g_interactive
	return true
}

func main() {

	var conf Configure
	var db *sql.DB

	if false == parse_input_args(&conf) {
		return
	}
	if conf.Database {
		var db_path string

		if user, err := user.Current(); nil == err {
			db_path = user.HomeDir + "/.dictionary.sqlite"
		} else {
			fmt.Printf("%v\n", err)
			return
		}
		if db_res, err := db_open(db_path); err != nil {
			fmt.Printf("%v\n", err)
		} else {
			db = db_res
			defer db.Close()
		}
	}
	if false == conf.Interactive {
		query_and_show(db, conf, *g_word)
	} else {
		run_interactive_mode(db, conf)
	}
}
