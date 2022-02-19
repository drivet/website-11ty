import json
import os
import yaml
import datetime
import re
import pytz
from dateutil.parser import parse


# pass in the properties of the h-entry
def hentry_to_eleventy(hentry):
    date = localize_date(get_single_prop(hentry, 'published'))
    modified = localize_date(get_single_prop(hentry, 'updated'))
    title = get_single_prop(hentry, 'name')
    blurb = get_single_prop(hentry, 'summary')
    tags = get_list_prop(hentry, 'category')

    meta = {}
    if date:
        meta['date'] = date
    if modified:
        meta['modified'] = modified
    if title:
        meta['title'] = title
    if blurb:
        meta['blurb'] = blurb
    if tags:
        meta['tags'] = tags

    meta['mp_properties'] = hentry
    content = get_single_prop(hentry, 'content')
    if 'content' in hentry:
        del hentry['content']
    return meta, content

    
def get_single_prop(p, prop):
    if prop in p and p[prop]:
        return p[prop][0]
    else:
        return None

    
def get_list_prop(p, prop):
    return p.get(prop, [])


def localize_date(datestr):
    if not datestr:
        return None
    d = parse(datestr)
    if d.tzinfo is None or d.tzinfo.utcoffset(d) is None:
        d = pytz.timezone("US/Eastern").localize(d)
    return d


def convert_notedown(text):
    # Only match whitespace that has whitespace behind it (lookbehind)
    multispace_re = re.compile(r"(?<=\s)\s")
    
     # we'll say, somewhat arbitrarily, that tabs are replaced by four spaces
    text = text.replace('\t', '&nbsp;&nbsp;&nbsp;&nbsp;')

    # replace newlines and carriage returns with HTML breaks
    text = text.replace('\r\n', '<br />')
    text = text.replace('\n', '<br />')
    
    # replace more than two spaces with escape codes
    text = multispace_re.sub('&nbsp;', text)

    # replace line breaks with double spaces
    text = text.replace('<br />', '  ')
    
    return text


def read_whole_file(filename):
    with open(filename, 'r') as content_file:
        content = content_file.read()
    return content


files = []
inputdir = '/home/dcr/11tyblog/content_orig/'
for root, directories, filenames in os.walk(inputdir):
    files += [os.path.join(root, f) for f in filenames if f.endswith('.mp')]

outputdir = '/home/dcr/11tyblog/content'
for filename in files:
    f = json.loads(read_whole_file(filename))
    meta, content = hentry_to_eleventy(f['properties'])

    output = os.path.join(outputdir, filename[len(inputdir):])
    output = re.sub('\.mp$', '.md', output)
    dirname = os.path.dirname(output)
    if not os.path.exists(dirname):
        os.makedirs(dirname)
    f = open(output, 'w')
    f.write('---\n')
    f.write(yaml.dump(meta))
    f.write('---\n\n')
    if content:
        f.write(convert_notedown(content))
    f.close()
