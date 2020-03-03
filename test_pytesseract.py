"""
    Test pytesseract which wraps around the CLI process
"""

from PIL import Image
import pytesseract
import concurrent.futures

image = Image.open('build/1fe1a264bae392cd56f89c647c71dd11_page_1_.jpg')

def run(image, ocr):
   print(pytesseract.image_to_string(image))

with concurrent.futures.ThreadPoolExecutor(max_workers = 10) as executor:
    f = {executor.submit(run, image.copy(), pytesseract): i for i in range(100)}
    for future in concurrent.futures.as_completed(f):
        res = f[future]
        try:
            data = future.result()
        except Exception as exc:
            print('%r generated an exception: %s' % (exc))

#
# DONE
