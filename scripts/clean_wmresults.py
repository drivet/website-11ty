import yaml
with open('./src/_data/wmresults.yaml', 'r') as file:
  wmresults = yaml.safe_load(file)

results = wmresults['results']
print("precount "+str(len(results.keys())))
for data in results.values():
  for k in list(data.keys()):
    if k.startswith('https://brid.gy/publish/twitter'):
      if 'location' in data[k] and data[k]['location'].startswith('https://twitter.com/desmondrivet'):
        del data[k]
      elif 'status_code' in data[k] and data[k]['status_code'] >= 400:
        del data[k]

print("count "+str(len(wmresults['results'])))
with open('./src/_data/wmresults.yaml', 'w') as file:
    yaml.dump(wmresults, file)
