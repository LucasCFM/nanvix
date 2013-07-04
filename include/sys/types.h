/*
 * Copyright (C) 2011-2013 Pedro H. Penna <pedrohenriquepenna@gmail.com>
 *
 * types.h - Data types
 */

#ifndef TYPES_H_
#define TYPES_H_

#ifndef _ASM_FILE_

	/* Used for device IDs. */
	typedef unsigned dev_t;
	
	/* Used as a general identifier. */
	typedef int id_t;
	
	/* Used for some file attributes. */
	typedef int gid_t;
	
	/* Used for some file attributes. */
	typedef int mode_t;
	
	/* Used for file sizes. */
	typedef signed off_t;

	/* Used for process IDs and process group IDs. */
	typedef signed pid_t;

	/* Used for sizes of objects. */
	#ifndef SIZE_T
	#define SIZE_T
		typedef unsigned size_t;
	#endif
	
	/* Used for a count of bytes or an error indication. */
	typedef signed ssize_t;
	
	/* Used for user IDs. */
	typedef int uid_t;
	
#endif /* _ASM_FILE */

#endif /* TYPES_H_ */
