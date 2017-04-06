#!/usr/bin/env python
# -*- coding: utf-8 -*-

import argparse
import re
import sys

LEFT_PAR = 1
RIGHT_PAR = 2
LEFT_CURL = 3
RIGHT_CURL = 4
ID = 5
EMPTY_STR = 6
ARROW = 7
COMMA = 8
DOT = 9
SPACE = 10


class Token(object):
    instances = []

    def __init__(self, data, type):
        self.data = data
        self.type = type
        Token.instances.append(self)


def check_args():
    parser = argparse.ArgumentParser(
        description='IPP 2'
    )
    parser.add_argument(
        '--input',
        help='if not set it takes STDIN',
        default=sys.stdin
    )
    parser.add_argument(
        '--output',
        help='if not set output will be at STDOUT',
        default=sys.stdout
    )
    parser.add_argument(
        '-m',
        '--minimize',
        help='minimizes FSM',
        action="store_true"
    )
    parser.add_argument(
        '-f',
        '--find-non-finishing',
        help='find states and prints them otherwise \'0\'',
        action="store_true"
    )
    parser.add_argument(
        '-i',
        '--case-insensitive',
        help='ignoring input states strings case',
        action="store_true"
    )

    try:
        args = parser.parse_args()
    except:
        exit(1)
    if args.find_non_finishing == args.minimize and args.minimize is True:
        exit_error("Wrong combination of arguments", 1)
    if (sys.argv[2] == "--help"):
        if (len(sys.argv) == 2):
            print(parser.format_help())
            exit(0)
        else:
            exit_error("Wrong combination of arguments", 1)
    return args


def input_file(file):
    try:
        if file:
            with open(file, 'r') as fd:
                file_content = fd.read()
        else:
            file_content = sys.stdin.read()
    except FileNotFoundError:
        exit_error('Failed to open input file.', 2)
    return file_content


def output_file(file, output):
    try:
        if output_file:
            with open(file, 'w') as fd:
                fd.write(output)
        else:
            sys.stdout.write(output)
    except (FileNotFoundError, PermissionError):
        exit_error('Failed to open output file.', 3)


def exit_error(msg, errno):
    sys.stderr.write(msg)
    sys.exit(errno)


def normalize_input(string):
    string = string.replace("'#'", "'&_octothorpe'")
    comment_regex = re.compile(r'#[^\n]*\n')
    end_comment_regex = re.compile(r'#[^$]*$')
    # remove comments
    string = comment_regex.sub('', string)
    string = end_comment_regex.sub('', string)
    string = string.replace("'&_octothorpe'", "'#'", )

    # remove all whitespaces
    string = ''.join(string.split())

    return string


def tokenize(input_data):
    token_stream = []

    data_len = len(input_data)
    if input_data[data_len - 1] != ")" or input_data[data_len - 2] != "}":
        exit_error("Lexical error", 61)

    i = 0
    while i < data_len:
        current_char = input_data[i]
        i += 1
        if current_char == "(":
            token_stream.append(Token(current_char, LEFT_PAR))
        elif current_char == ")":
            token_stream.append(Token(current_char, RIGHT_PAR))
        elif current_char == "{":
            token_stream.append(Token(current_char, LEFT_CURL))
        elif current_char == "}":
            token_stream.append(Token(current_char, RIGHT_CURL))
        elif current_char == "'":
            current_char = input_data[i]
            if current_char == "'":
                i += 1
                current_char = input_data[i]
                if current_char == "'":
                    i += 1
                    current_char = input_data[i]
                    if current_char == "'":
                        token_stream.append(Token("''", EMPTY_STR))
                    else:
                        exit_error("Lexical error", 60)
                else:
                    i -= 1
                    exit_error("Lexical error", 60)
            else:
                pass

        elif current_char == "-":
            current_char = input_data[i]
            if current_char == ">":
                token_stream.append(Token("->", ARROW))
                i += 1
            else:
                exit_error("Lexical error", 60)
        elif current_char == ",":
            token_stream.append(Token(current_char, COMMA))
        elif current_char == ".":
            token_stream.append(Token(current_char, DOT))
        elif current_char == " ":
            token_stream.append(Token(current_char, SPACE))
        else:
            value = ""
            if re.search("(\w+)", current_char):
                value += current_char
                while i < data_len:
                    current_char = input_data[i]
                    if re.search("(\w+)", current_char):
                       value += current_char
                    elif current_char == "," or current_char == "'" or current_char == "}":
                        break
                    i += 1
            token_stream.append(Token(value, ID))

    return token_stream


def syntax(token_stream):
    states = set()
    check = 0
    counter = 0

    id_regex = re.compile('[a-zA-Z]([_a-zA-Z0-9][a-zA-Z0-9])*')

    for token in token_stream:
        counter += 1
        if token.type == LEFT_PAR:
            continue
        if token.type == LEFT_CURL:
            continue
        if token.type == ID:
            if not re.match(id_regex, token.data):
                exit_error("Invalid ID", 60)
            states.add(token.data)
        if token.type == COMMA:
            continue
        if token.type == RIGHT_CURL:
            check += 1
            break


    print(states)
    print(counter)




if __name__ == '__main__':
    args = check_args()

    input_str = input_file(args.input)

    normalized = normalize_input(input_str)
    print(normalized)

    token_stream = tokenize(normalized)

    syntax(token_stream)
