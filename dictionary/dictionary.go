package main

import (
	"encoding/xml"
	"fmt"
	"io/ioutil"
	"net/http"
	"os/exec"
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
	if read && 1 < len(dict.Pron) {
		fmt.Printf("\n🔊")
		shell_cmd("play " + dict.Pron[1])
	}
	fmt.Printf("\n")
}
