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