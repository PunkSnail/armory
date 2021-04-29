package main

import (
	"encoding/xml"
	"flag"
	"fmt"
	"io/ioutil"
	"net/http"
	"os"
	"strings"

	"github.com/fatih/color"
)

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

func parse_input_args() bool {

	var result bool = false

	if 1 == len(os.Args) {
		fmt.Printf("missing args, try: %s --help\n", os.Args[0])
		return result
	}
	if flag.Parse(); len(*g_word) < 1 {
		fmt.Println("missing single word")
		return result
	}
	result = true
	return result
}

func online_get_word(url_prefix string, word string) ([]byte, error) {

	response, err := http.Get(url_prefix + word)
	if err != nil {
		return nil, err
	}
	defer response.Body.Close()

	return ioutil.ReadAll(response.Body)
}

func show_get_result(data []byte, show_url bool) {

	var dict Dict
	xml.Unmarshal(data, &dict)

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
	fmt.Printf("\n")
}
