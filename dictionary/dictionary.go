package main

import (
	"bufio"
	"database/sql"
	"encoding/xml"
	"fmt"
	"io/ioutil"
	"net/http"
	"os"
	"os/exec"
	"strings"

	"github.com/fatih/color"
)

type Configure struct {
	External    bool
	Sound       bool
	Database    bool
	Interactive bool
}

type Dict struct {
	Key         string   `xml:"key"`
	Ps          []string `xml:"ps"`
	Pron        []string `xml:"pron"`
	Pos         []string `xml:"pos"`
	Note        []string `xml:"note"`
	Acceptation []string `xml:"acceptation"`
	SentList    []Sent   `xml:"sent"`
}

type Sent struct {
	Orig  string `xml:"orig"`
	Trans string `xml:"trans"`
}

func online_get_word(url_prefix string, word string) ([]byte, error) {

	response, err := http.Get(url_prefix + word)
	if err != nil {
		return nil, err
	}
	defer response.Body.Close()

	return ioutil.ReadAll(response.Body)
}

/* Using the shell is a bit ugly, but it works */
func shell_cmd(cmd string) ([]byte, error) {

	exec_cmd := exec.Command("/bin/bash", "-c", cmd)

	return exec_cmd.CombinedOutput()
}

func presenting_result(data []byte, show_url bool, read bool) {

	var dict Dict

	if err := xml.Unmarshal(data, &dict); err != nil {
		fmt.Printf("%v", err)
		return
	}
	fmt.Printf("\n%s\t", dict.Key)
	if 1 < len(dict.Ps) {
		color.Magenta("美[%s]\t英[%s]\n\n", dict.Ps[1], dict.Ps[0])
	} else {
		fmt.Printf("\n\n")
	}
	for i, pos := range dict.Pos {
		if 0 < len(pos) {
			color.Yellow("%s %s", strings.TrimSpace(pos),
				strings.TrimSpace(dict.Acceptation[i]))
		}
	}
	for i := 0; i < len(dict.SentList); i++ {
		if 0 == i {
			fmt.Printf("\n场景例句:\n")
		}
		sent := dict.SentList[i]
		fmt.Printf("[%d]. %s\n", i+1, strings.TrimSpace(sent.Orig))
		color.Cyan("    %s", strings.TrimSpace(sent.Trans))
	}
	if show_url && 1 < len(dict.Pron) {
		fmt.Printf("\n发音:\n美[%s]\n英[%s]\n",
			dict.Pron[1], dict.Pron[0])
	}
	if read && 1 < len(dict.Pron) {
		fmt.Printf("\n🔊")
		shell_cmd("play " + dict.Pron[1])
	}
	fmt.Printf("\n")
}

func play_directly(data []byte) {

	var dict Dict
	err := xml.Unmarshal(data, &dict)

	if err == nil && 1 < len(dict.Pron) {
		fmt.Printf("🔊")
		shell_cmd("play " + dict.Pron[1])
	} else {
		fmt.Printf("%v", err)
	}
}

func query_and_show(db *sql.DB, conf Configure, word string) []byte {

	var db_data string
	var result []byte
	if conf.Database {
		db_data = db_get_word(db, word)
	}
	if 0 == len(db_data) {
		url_prefix := DICT_URL + "?key=" + DICT_KEY + "&w="

		if data, err := online_get_word(url_prefix, word); nil == err {

			if conf.Database && strings.Contains(string(data), "sent") {
				// example sentence exists, save data to the database
				db_set_word(db, word, string(data))
			}
			presenting_result(data, conf.External, conf.Sound)
			result = data
		} else {
			fmt.Printf("%v\n", err)
		}
	} else {
		result = []byte(db_data)
		presenting_result([]byte(db_data), conf.External, conf.Sound)
	}
	return result
}

func run_interactive_mode(db *sql.DB, conf Configure) {

	fmt.Println("输入 \"QUIT\" 或 \"q\" 退出。type \"QUIT\" or \"q\" to exit.")
	fmt.Printf("输入 \"REPEAT\" 或 \"r\" 播放上一个词。%s",
		"type \"REPEAT\" or \"r\" to play the last word.")

	var last_cache []byte
	reader := bufio.NewReader(os.Stdin)

read_start:
	fmt.Printf("\n> ")
	input, err := reader.ReadString('\n')
	if err != nil {
		fmt.Printf("read string faild %v\n", err)
		return
	} else {
		input = input[:len(input)-1] // delete '\n'
	}
	if input == "q" || input == "QUIT" || input == "Q" {
		return
	}
	if input == "r" || input == "REPEAT" || input == "R" {
		play_directly(last_cache)
	} else {
		last_cache = query_and_show(db, conf, input)
	}
	goto read_start
}
