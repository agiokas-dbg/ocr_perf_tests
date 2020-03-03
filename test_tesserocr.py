"""
    Test tesserocr which uses CPython bindings
"""
import tesserocr
from PIL import Image
import concurrent.futures


print(tesserocr.tesseract_version())  # print tesseract-ocr version
print(tesserocr.get_languages())  # prints tessdata path and list of available languages

image = Image.open('build/1fe1a264bae392cd56f89c647c71dd11_page_1_.jpg')

def run(image, ocr):
   print(ocr.image_to_text(image))

with concurrent.futures.ThreadPoolExecutor(max_workers = 10) as executor:
    f = {executor.submit(run, image.copy(), tesserocr): i for i in range(100)}
    for future in concurrent.futures.as_completed(f):
        res = f[future]
        try:
            data = future.result()
        except Exception as exc:
            print('%r generated an exception: %s' % (exc))


#
# DONE
