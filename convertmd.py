import os
import markdown
import yaml
import datetime
import re
import pytz
from dateutil.parser import parse

def extract_markdown_metadata(metadata_text):
    md = markdown.Markdown(extensions=['meta'])
    md.convert(metadata_text)

    metadata = {}
    for key, value in md.Meta.items():
        metadata[key] = "\n".join(value)
    return metadata

def read_whole_file(filename):
    with open(filename, 'r') as content_file:
        content = content_file.read()
    return content


def localize_date(datestr):
    if not datestr:
        return None
    d = parse(datestr)
    if d.tzinfo is None or d.tzinfo.utcoffset(d) is None:
        d = pytz.timezone("US/Eastern").localize(d)
    return d


files = []
inputdir = '/home/dcr/11tyblog/content_orig/'
for root, directories, filenames in os.walk(inputdir):
    files += [os.path.join(root, f) for f in filenames if f.endswith('.md')]

outputdir = '/home/dcr/11tyblog/content'
for filename in files:
    f = read_whole_file(filename)
    meta_content = f.split('\n\n', 1)
    meta = extract_markdown_metadata(meta_content[0] + "\n\n")
    if 'tags' in meta:
        meta['tags'] = meta['tags'].split(',')

    timezone = pytz.timezone("US/Eastern")
    if 'date' in meta:
        meta['date'] = localize_date(meta['date'])
    if 'modified' in meta:
        meta['modified'] = localize_date(meta['modified'])

    output = os.path.join(outputdir, filename[len(inputdir):])
    dirname = os.path.dirname(output)
    if not os.path.exists(dirname):
        os.makedirs(dirname)
    f = open(output, 'w')
    f.write('---\n')
    f.write(yaml.dump(meta))
    f.write('---\n\n')
    f.write(meta_content[1])
    f.close()
    
