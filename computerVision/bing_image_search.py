#import necessary packages
from requests import exceptions
import argparse
import requests
import cv2
import os

#construct argument parser and parse arguments
ap = argparse.ArgumentParser()
ap.add_argument("-q", "--query", required=True, help="serach query to search Bing Image API for")
ap.add_argumen("-o", "--output", required=True, help="path to output directory of images")
args = vars(ap.parse_args())

API_KEY = ""
MAX_RESULTS = 250
GROUP_SIZE = 50
 
# set the endpoint API URL
URL = "https://api.cognitive.microsoft.com/bing/v5.0/images/search"

# when attempting to download images from the web both the Python
# programming language and the requests library have a number of
# exceptions that can be thrown so let's build a list of them now
# so we can filter on them
EXCEPTIONS = set([IOError, FileNotFoundError,
	exceptions.RequestException, exceptions.HTTPError,
	exceptions.ConnectionError, exceptions.Timeout])

# store the search term in a convenience variable then set the
# headers and search parameters
term = args["query"]
headers = {"Ocp-Apim-Subscription-Key" : API_KEY}
params = {"q": term, "offset": 0, "count": GROUP_SIZE}
 
# make the search
print("[INFO] searching Bing API for '{}'".format(term))
search = requests.get(URL, headers=headers, params=params)
search.raise_for_status()
 
# grab the results from the search, including the total number of
# estimated results returned by the Bing API
results = search.json()
estNumResults = min(results["totalEstimatedMatches"], MAX_RESULTS)
print("[INFO] {} total results for '{}'".format(estNumResults,
    term))
 
# initialize the total number of images downloaded thus far
total = 0