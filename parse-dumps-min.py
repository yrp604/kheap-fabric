#!/usr/bin/env python

import json
import os
import sys

from HTMLParser import HTMLParser

def usage(argv0):
	sys.stderr.write('Usage: %s <path/to/report-dir>\n' % argv0)
	sys.exit(1)

def log(s):
	sys.stderr.write(s + '\n')

# example:
#     0 | struct zs_pool
#     0 |   const char * name
#     8 |   struct size_class *[255] size_class
#  2048 |   struct kmem_cache * handle_cachep
#  2056 |   struct kmem_cache * zspage_cachep
#  2064 |   atomic64_t pages_allocated
#  2064 |     long counter
#  2072 |   struct zs_pool_stats stats
#  2072 |     unsigned long pages_compacted
#  2080 |   struct shrinker shrinker
#  2080 |     unsigned long (*)(struct shrinker *, struct shrink_control *) count_objects
#  2088 |     unsigned long (*)(struct shrinker *, struct shrink_control *) scan_objects
#  2096 |     int seeks
#  2104 |     long batch
#  2112 |     unsigned long flags
#  2120 |     struct list_head list
#  2120 |       struct list_head * next
#  2128 |       struct list_head * prev
#  2136 |     atomic_long_t * nr_deferred
#  2144 |   bool shrinker_enabled
#  2152 |   struct inode * inode
#  2160 |   struct work_struct free_work
#  2160 |     atomic64_t data
#  2160 |       long counter
#  2168 |     struct list_head entry
#  2168 |       struct list_head * next
#  2176 |       struct list_head * prev
#  2184 |     work_func_t func
#       | [sizeof=2192, align=8]
def convert(layout):
	def split_line(l):
		(offset, decl) = l.split(' | ')

		# bitfield hack?
		if ':' in offset: offset = offset.split(':')[0]

		try:
			offset = int(offset)
		except ValueError:
			# [sizeof=2192, align=8]
			sz = decl.split(',')[0].split('=')[-1]
			offset = int(sz)

		return (offset, decl)

	lines = layout.strip().split('\n')

	stack = []

	for i in range(len(lines) - 1):
		off, decl   = split_line(lines[i])
		next_off, _ = split_line(lines[i+1])

		sz = next_off - off

		indent = decl.count('  ')

		path = decl.split(' ')[-1]
		if path == '': path = '<anon>'

		type_sig = ' '.join(decl.lstrip('  ').split(' ')[:-1])

		for x in reversed(stack):
			if x['indent'] == indent - 1:
				path = x['path'] + '.' + path
				break

		stack.append({'type': type_sig, 'path': path, 'offset': off, 'size': sz, 'indent': indent})

	map(lambda e: e.pop('indent'), stack)

	total_sz, _ = split_line(lines[-1])

	layout = filter(lambda e: e['size'] > 0, stack)

	return {'size': total_sz, 'layout': layout}

class ReportParser(HTMLParser):
	def __init__(self):
		self.filename    = None
		self.lineno      = None
		self.struct_name = None
		self.struct_def  = None

		self.state = -1
		
		# super() doesn't work here for some reason
		HTMLParser.__init__(self)

	def dump(self):
		return {'filename'   : self.filename,
			    'lineno'     : self.lineno,
			    'struct_name': self.struct_name,
			    'struct_def' : self.struct_def}

	def handle_starttag(self, tag, attrs):
		if tag == 'td': self.state += 1

	# Ok, this is ghetto parsing
	# <tr><td class="rowname">File:</td><td>/home/joe/linux-4.14/fs/btrfs/ctree.c</td></tr>
	# <tr><td class="rowname">Warning:</td><td><a href="#EndPath">line 696, column 7</a><br />Struct used in dynamic allocation</td></tr>
	# <tr><td></td><td>struct tree_mod_elem</td></tr>
	# <tr><td></td><td>         0 | struct tree_mod_elem
	#          0 |   struct rb_node node
	# ...
	#            | [sizeof=112, align=8]
	# </td></tr>
	# 
	# counting table cells
	# 0: first tag
	# 1: filename
	# ...
	# 3: line number
	# 5: struct name
	# 7: definition
	# ...
	def handle_data(self, data):
		if self.state == 1 and self.filename == None:
			self.filename = data
		if self.state == 3 and self.lineno == None:
			self.lineno = int(data.split(' ')[1].strip(','))
		if self.state == 5 and self.struct_name == None:
			self.struct_name = data
		if self.state == 7 and self.struct_def == None:
			self.struct_def = data

def parse_report(report):
	parser = ReportParser()
	parser.feed(report)

	info = parser.dump()

	info['struct_def'] = convert(info['struct_def'])

	return info

def main(argv):
	if len(argv) != 2: usage()

	report_dir = argv[1]

	structs = {}

	for fname in os.listdir(report_dir):
		if 'report' not in fname: continue

		path = os.path.join(report_dir, fname)

		log('processing %s...' % path)

		with open(path, 'r') as h:
			struct_info = parse_report(h.read())

		loc = {
			'filename': struct_info['filename'],
			'lineno': struct_info['lineno'],
			'report': fname}

		name = struct_info['struct_name']

		if name not in structs:
			structs[name] = {
				'loc': [loc],
				'layout': struct_info['struct_def']['layout'],
				'size': struct_info['struct_def']['size']}
		else:
			structs[name]['loc'].append(loc)

	log('got %d structures' % len(structs))

	print(json.dumps(structs, indent=True))

if __name__ == '__main__':
	main(sys.argv)
