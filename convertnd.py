import os
import markdown
import yaml
import datetime
import re
import pytz
    
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
    
files = []
inputdir = '/home/dcr/11tyblog/content_orig/'
for root, directories, filenames in os.walk(inputdir):
    files += [os.path.join(root, f) for f in filenames if f.endswith('.nd')]

outputdir = '/home/dcr/11tyblog/content'
for filename in files:
    f = read_whole_file(filename)
    meta_content = f.split('\n\n', 1)
    meta = extract_markdown_metadata(meta_content[0] + "\n\n")
    if 'tags' in meta:
        meta['tags'] = meta['tags'].split(',')

    # don't really want hardcoded hashtags
    if 'hashtags' in meta:
        del meta['hashtags']

    timezone = pytz.timezone("US/Eastern")
    if 'date' in meta:
        meta['date'] = re.sub(r'\.\d\d\d\d\d\d$', '', meta['date'])
        meta['date'] = datetime.datetime.strptime(meta['date'], "%Y-%m-%d %H:%M:%S")
        meta['date'] = timezone.localize(meta['date'])
    if 'modified' in meta:
        meta['modified'] = re.sub(r'\.\d\d\d\d\d\d$', '', meta['modified'])
        meta['modified'] = datetime.datetime.strptime(meta['modified'], "%Y-%m-%d %H:%M:%S")
        meta['modified'] = timezone.localize(meta['modified'])

    output = os.path.join(outputdir, filename[len(inputdir):])
    output = re.sub('\.nd$', '.md', output)
    dirname = os.path.dirname(output)
    if not os.path.exists(dirname):
        os.makedirs(dirname)
    f = open(output, 'w')
    f.write('---\n')
    f.write(yaml.dump(meta))
    f.write('---\n\n')
    f.write(convert_notedown(meta_content[1]))
    f.close()
    
