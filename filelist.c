// filelist.c - Functions supporting the file link list

filenode *filelist, *fn;

// Clear the list
void filelist_init()
{
	fn = NULL;
	filelist = fn;
}

// Add a filename to the list
void filelist_add(char * filename)
{
	fn = filelist;
	if(fn == NULL) {
		// File list is empty
		fn = new filenode;
	} else {
		// File list not empty.  Insert new node at end.
		while(fn.next  != NULL)
			fn = fn.next;
		fn.next = new filenode;
		fn = fn.next;
	}
	strcpy(fn.filename, filename);
	fn.sentcount = 0;
	fn.timeadded = 0;
	fn.next = NULL;
}

// Remove a filename from the list
void filelist_remove(char * filename)
{
	filenode *old;
	fn = filelist;
	if(fn == NULL) 
		// List is empty.  Do nothing.
	else {
		while(fn.next != NULL && strcmp(fn.next.filename, filename) {
			if(fn.next == NULL)
				// Last node on the list.  Do nothing
			else {
				// Found a match (next record)
				old = fn.next;
				fn.next = fn.next.next;
				delete old;
			}
		}
	}
}
