# import requests
# from bs4 import BeautifulSoup
# import argparse
#
# def genius_parser(artist_name, song_name):
#     artist_name = artist_name.capitalize()
#     artist_name = artist_name.replace(" ", "-")
#     song_name = song_name.replace(" ", "-")
#     url = (f'https://genius.com/{artist_name}-{song_name}-lyrics')
#     response = requests.get(url)
#     soup = BeautifulSoup(response.text, "lxml")
#     song_text = soup.find("div", class_="Lyrics__Container-sc-1ynbvzw-1 kUgSbL").get_text(strip=True, separator='\n')
#     # song_text = song_text.split("<br")
#     # song_text  = list(map(lambda x: x.text, song_text.split("<br")))
#
#     if response.status_code != 200:
#         raise Exception(f"Failed to fetch lyrics. Status code: {response.status_code}")
#
#     soup = BeautifulSoup(response.text, "lxml")
#     lyrics_container = soup.find("div", class_="Lyrics__Container-sc-1ynbvzw-1 kUgSbL")
#
#     if not lyrics_container:
#         lyrics_container = soup.find("div", class_="lyrics")
#
#         if not lyrics_container:
#             raise Exception("Failed to parse lyrics. Container not found.")
#
#     song_text = lyrics_container.get_text(strip=True, separator='\n')
#     return song_text
#
#
# parser = argparse.ArgumentParser(
#                     prog='genius_parser',
#                     description='Programme parse lyrics from genius.com',
#                     epilog='')
#
# parser.add_argument('artist_name', help='Artist name')
# parser.add_argument("song_name", help="Song name")
#
#
# args = parser.parse_args()
#
#
# lyrics = genius_parser(args.artist_name, args.song_name)
# print(lyrics)
# print (genius_parser("Eminem", "Stan"))
# "Lyrics__Container-sc-1ynbvzw-1 kUgSbL"

import requests
from bs4 import BeautifulSoup
import argparse


def genius_parser(artist_name, song_name):
    artist_name_str = "-".join(artist_name)
    song_name_str = "-".join(song_name)
    url = f'https://genius.com/{artist_name_str}-{song_name_str}-lyrics'
    print(f"Generated URL: {url}")
    response = requests.get(url)

    if response.status_code != 200:
        raise Exception(f"Failed to fetch lyrics. Status code: {response.status_code}")

    soup = BeautifulSoup(response.text, "lxml")
    lyrics_container = soup.find("div", class_="Lyrics__Container-sc-1ynbvzw-1 kUgSbL")

    if not lyrics_container:
        lyrics_container = soup.find("div", class_="lyrics")

        if not lyrics_container:
            raise Exception("Failed to parse lyrics. Container not found.")

    song_text = lyrics_container.get_text(strip=True, separator='\n')
    return song_text


parser = argparse.ArgumentParser(
    prog='genius_parser',
    description='Program to parse lyrics from genius.com',
    epilog='')

parser.add_argument('artist_name', nargs='+', help='Artist name')
parser.add_argument("song_name", nargs='+', help="Song name")

args = parser.parse_args()

try:
    lyrics = genius_parser(args.artist_name, args.song_name)
    print(lyrics)
except Exception as e:
    print(str(e))