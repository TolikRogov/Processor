#include "../include/STACK_LIBRARY.hpp"

StackStatusCode HtmlLogStarter(Stack_t* stk) {

	StackStatusCode status = STACK_NO_ERROR;

	status = DirCtor(stk);
	STACK_ERROR_CHECK(status, stk);

	status = FilesCtor(stk);
	STACK_ERROR_CHECK(status, stk);

	return STACK_NO_ERROR;
}

StackStatusCode DirCtor(Stack_t* stk) {

	StackStatusCode status = STACK_NO_ERROR;

	status = DeleteLogDir(stk);
	STACK_ERROR_CHECK(status, stk);

	status = MakeDirsPaths(stk);
	STACK_ERROR_CHECK(status, stk);

	status = MakeFilesPaths(stk);
	STACK_ERROR_CHECK(status, stk);

	status = MakeDirsFolders(stk);
	STACK_ERROR_CHECK(status, stk);

	return STACK_NO_ERROR;
}

StackStatusCode MakeDirsPaths(Stack_t* stk) {

	StackStatusCode status = STACK_NO_ERROR;

	status = StrConcatenation(stk->log.dir_name, stk->log.styles.c_dir_name, &stk->log.styles.c_dir_path, stk);
	STACK_ERROR_CHECK(status, stk);

	return STACK_NO_ERROR;
}

StackStatusCode MakeFilesPaths(Stack_t* stk) {

	StackStatusCode status = STACK_NO_ERROR;

	for (size_t i = 0; i < stk->log.cnt_files; i++) {
		status = StrConcatenation(stk->log.dir_name, stk->log.files[i].file_name, &stk->log.files[i].file_path, stk);
		STACK_ERROR_CHECK(status, stk);
	}

	for (size_t i = 0; i < stk->log.styles.c_dir_cnt_files; i++) {
		status = StrConcatenation(stk->log.styles.c_dir_path, stk->log.styles.files[i].file_name, &stk->log.styles.files[i].file_path, stk);
		STACK_ERROR_CHECK(status, stk);
	}

	return STACK_NO_ERROR;
}

StackStatusCode MakeDirsFolders(Stack_t* stk) {

	StackStatusCode status = STACK_NO_ERROR;

	const char* folders[] = { stk->log.dir_name,
							  stk->log.styles.c_dir_path };

	size_t size = sizeof(folders) / sizeof(folders[0]);

	for (size_t i = 0; i < size; i++) {
		char* make_folder = NULL;
		status = StrConcatenation("mkdir ", folders[i], &make_folder, stk);
		STACK_ERROR_CHECK(status, stk);

		DIR* cur_dir = opendir(folders[i]);
		if (cur_dir) {
			if(closedir(cur_dir))
				STACK_ERROR_CHECK(STACK_DIR_CLOSE_ERROR, stk);
			continue;
		}

		if(system(make_folder))
			STACK_ERROR_CHECK(STACK_DIR_MAKE_ERROR, stk);
		if (make_folder) {
			free(make_folder);
			make_folder = NULL;
		}
	}

	return STACK_NO_ERROR;
}

StackStatusCode DeleteLogDir(Stack_t* stk) {

	StackStatusCode status = STACK_NO_ERROR;

	DIR* log_dir = opendir(stk->log.dir_name);
	if (!log_dir)
		STACK_ERROR_CHECK(STACK_DIR_OPEN_ERROR, stk);

	if(closedir(log_dir))
		STACK_ERROR_CHECK(STACK_DIR_CLOSE_ERROR, stk);

	char* delete_folder = NULL;
	status = StrConcatenation("rm -rf ", stk->log.dir_name, &delete_folder, stk);
	STACK_ERROR_CHECK(status, stk);

	if(system(delete_folder))
		STACK_ERROR_CHECK(STACK_DIR_DELETE_ERROR, stk);
	if (delete_folder) {
		free(delete_folder);
		delete_folder = NULL;
	}

	log_dir = opendir(stk->log.dir_name);
	if (log_dir) {
		if(closedir(log_dir))
			STACK_ERROR_CHECK(STACK_DIR_CLOSE_ERROR, stk);
#ifdef N_DEBUG
		printf("\n\n" RED("LOG DIR WAS NOT REMOVED\n") "\n\n");
#endif
	}
#ifdef N_DEBUG
	else
		printf("\n\n" GREEN("LOG DIRECTORY WAS REMOVED!") "\n\n");
#endif

	return STACK_NO_ERROR;
}

StackStatusCode FilesCtor(Stack_t* stk) {

	StackStatusCode status = STACK_NO_ERROR;

	for (size_t i = 0; i < stk->log.cnt_files; i++) {

		FILE* log_file = fopen(stk->log.files[i].file_path, "w");
		if (!log_file)
			STACK_ERROR_CHECK(STACK_FILE_OPEN_ERROR, stk);

		fprintf(log_file, "<!DOCTYPE HTML PUBLIC>\n");
		fprintf(log_file, "<html>\n");

		fprintf(log_file, "\t<head>\n");
		fprintf(log_file, "\t\t<title>Stack Dump</title>\n");

		char* include_style = NULL;
		status = StrConcatenation(stk->log.styles.c_dir_name, stk->log.styles.files[CSS].file_name, &include_style, stk);
		STACK_ERROR_CHECK(status, stk);

		fprintf(log_file, "\t\t<link rel='stylesheet' href='%s'>\n", include_style);
		if (include_style) {
			free(include_style);
			include_style = NULL;
		}

		fprintf(log_file, "\t</head>\n");

		status = CssLogStarter(stk);
		STACK_ERROR_CHECK(status, stk);

		fprintf(log_file, "\t<body>\n");
		fprintf(log_file, "\t\t<h1 align='center' name='top'><tt>MEGA DUMP</tt></h1>\n");
		fprintf(log_file, "\t\t<p><a href='#down' class='anchor'><button class='btn'><tt>Down</tt></button></a>\n");
		for (size_t j = 0; j < stk->log.cnt_files; j++)
			if (i != j)
				fprintf(log_file, "\t\t<a href='%s' class='anchor'><button class='btn'><tt>%s</tt></button></a>\n",
						stk->log.files[j].file_name, stk->log.files[j].file_name);
		fprintf(log_file, "\t\t</p><br>\n");

#ifdef HTML_DUMP
		fprintf(log_file, "\t\t<h2 style='text-decoration: underline'><p class = 'stack_info'>"
						 "<tt>Stack_t[<span class='address'>%p</span>] "
						 "born at <span class='stack_file_name'>%s</span>: <span class='stack_line'>%zu</span>, "
						 "name '<span class='stack_name'>%s</span>'</tt></p></h2>\n",
						 stk, stk->stack_info.file_name, stk->stack_info.line, stk->stack_info.stack_name);
#endif

		if (fclose(log_file))
			STACK_ERROR_CHECK(STACK_FILE_CLOSE_ERROR, stk);
	}

	status = HtmlTableLog(stk);
	STACK_ERROR_CHECK(status, stk);

	return STACK_NO_ERROR;
}

StackStatusCode CssLogStarter(Stack_t* stk) {

	FILE* styles_file = fopen(stk->log.styles.files[CSS].file_path, "w");
	if (!styles_file)
		STACK_ERROR_CHECK(STACK_FILE_OPEN_ERROR, stk);

	fprintf(styles_file, ".anchor { \n\toutline: none; \n\ttext-decoration: none; \
			\n\tpadding: 2px 1px 0; \n}\n");
	fprintf(styles_file, ".anchor:link { \n\tcolor: #fff; \n}\n\n");
	fprintf(styles_file, ".anchor:visited { \n\tcolor: #fff; \n}\n\n");
	fprintf(styles_file, ".anchor:focused { \n\tborder-bottom: 1px solid; \n\tbackground: #bae498; \n}\n\n");

	fprintf(styles_file, ".main_dump { \n\tbackground-color: #F7F7F7; \n\tpadding: 12px 30px;"
						"\n\t border-radius: 20px; \n\tmargin: 20px 100px; \n}\n\n");
	fprintf(styles_file, ".stack_info { \n\ttext-align: center; \n}\n\n");
	fprintf(styles_file, ".address { \n\ttext-align: center; \n\tcolor: #4B0082; \n}\n\n");
	fprintf(styles_file, ".stack_file_name { \n\ttext-align: center; \n\tcolor: #FFA500; \n}\n\n");
	fprintf(styles_file, ".stack_line { \n\ttext-align: center; \n\tcolor: #20B2AA; \n}\n\n");
	fprintf(styles_file, ".stack_name { \n\ttext-align: center; \n\tcolor: #7B68EE; \n}\n\n");
	fprintf(styles_file, ".stack_function { \n\ttext-align: center; \n\tcolor: #8B008B; \n}\n\n");
	fprintf(styles_file, ".main_dump_number { \n\ttext-align: center; \n\tcolor: #BDB76B; \n}\n\n");
	fprintf(styles_file, ".poison { \n\ttext-align: center; \n\tcolor: #006400; \n}\n\n");
	fprintf(styles_file, ".poison_value { \n\ttext-align: center; \n\tcolor: #FF0000; \n}\n\n");
	fprintf(styles_file, ".hex_speak { \n\ttext-align: center; \n\tcolor: #9400D3; \n}\n\n");
	fprintf(styles_file, ".hash { \n\ttext-align: center; \n\tcolor: #1E90FF; \n}\n\n");
	fprintf(styles_file, ".value { \n\ttext-align: center; \n\tcolor: #0000FF; \n}\n\n");
	fprintf(styles_file, ".parameter { \n\ttext-align: center; \n\tcolor: #8B4513; \n}\n\n");

	fprintf(styles_file, ".tb_stk { \n\twidth: 90%%; \n\tborder: 15px solid #F2F8F8; \
			\n\tborder-collapse: collapse; \n\tmargin: auto; \
			\n\ttable-layout: auto; \n\tmargin-bottom: 20px; \
			\n\tborder-top: 5px solid #F2F8F8; \n}\n\n");
	fprintf(styles_file, ".tb_stk th { \n\tfont-weight: bold; \n\tpadding: 5px;"
			"\n\tbackground: #F2F8F8; \n\tborder: none;"
			"\n\tborder-bottom: 5px solid #F2F8F8; \n}\n\n");
	fprintf(styles_file, ".tb_stk td { \n\tpadding: 10px; \n\tborder: none; \
			\n\tborder-bottom: 5px solid #F2F8F8; \n}\n\n");
	fprintf(styles_file, ".table_header > td { \n\ttext-align: center; \n}\n\n");
	fprintf(styles_file, ".tb_stk tbody tr:nth-child(odd) { \n\tbackground: #fff; \n}\n\n");
	fprintf(styles_file, ".tb_stk tbody tr:nth-child(even) { \n\tbackground: #F7F7F7; \n}\n\n");

	fprintf(styles_file, ".table_header > td > h3{ \n\tpadding: 10px 20px; \n}\n\n");
	fprintf(styles_file, ".stack_dump_number { \n\tcolor: turquoise; \n\ttext-align: center; \n}\n\n");
	fprintf(styles_file, ".stack_pointer { \n\tcolor: red; \n\ttext-align: center; \n}\n\n");
	fprintf(styles_file, ".stack_data_pointer { \n\tcolor: brown; \n\ttext-align: center; \n}\n\n");
	fprintf(styles_file, ".stack_data { \n\tcolor: green; \n\ttext-align: left; \n}\n\n");
	fprintf(styles_file, ".stack_capacity { \n\tcolor: blue; \n\ttext-align: center; \n}\n\n");
	fprintf(styles_file, ".stack_size { \n\tcolor: violet; \n\ttext-align: center; \n}\n\n");

	fprintf(styles_file, ".time { \n\tcolor: #4A235A; \n\tfont-size: 30px; \
			\n\tmargin: center; \n}\n\n");

	fprintf(styles_file, ".btn { \n\tborder: none; \n\tborder-radius: 15px; \n\ttext-decoration: none; \
    		\n\tcolor: white; \n\tbackground: #0B63F6; \n\tbox-shadow: 0 5px 0 #003CC5; \
			\n\tfont-size: 16px; \n\tpadding-block: 8px; \n\tpadding-inline: 15px; \n}\n\n");
	fprintf(styles_file, ".btn:hover { \n\tbackground: #003CC5; \n\tbox-shadow: none; \n\tposition: relative; \
    		\n\ttop: 5px; \n}\n\n");

	fprintf(styles_file, "h1[name='top'] { \n\tcolor: #4B0082; \n\tfont-style: italic; \
			\n\ttext-decoration: underline; \n}\n\n");

	if (fclose(styles_file))
		STACK_ERROR_CHECK(STACK_FILE_CLOSE_ERROR, stk);

	return STACK_NO_ERROR;
}

StackStatusCode HtmlTableLog(Stack_t* stk) {

	FILE* table_file = fopen(stk->log.files[TABLE].file_path, "a");
	if (!table_file)
		STACK_ERROR_CHECK(STACK_FILE_OPEN_ERROR, stk);

	fprintf(table_file, "\t\t<table class='tb_stk'>\n");
	fprintf(table_file, "\t\t\t<tr class='table_header'>\n");
	fprintf(table_file, "\t\t\t\t<td><tt><h3>DUMP #</tt></h3></td>\n");

#ifdef HASH_PROTECTION
	fprintf(table_file, "\t\t\t\t<td><tt><h3>Stack hash</tt></h3></td>\n");
#endif

#ifdef CANARY_PROTECTION
	fprintf(table_file, "\t\t\t\t<td><tt><h3>Stack canary1</tt></h3></td>\n");
#endif

	fprintf(table_file, "\t\t\t\t<td><tt><h3>Stack capacity</h3></tt></td>\n");
	fprintf(table_file, "\t\t\t\t<td><tt><h3>Stack size</h3></tt></td>\n");

#ifdef HASH_PROTECTION
	fprintf(table_file, "\t\t\t\t<td><tt><h3>Data hash</tt></h3></td>\n");
#endif

#ifdef CANARY_PROTECTION
	fprintf(table_file, "\t\t\t\t<td><tt><h3>Data canary1</tt></h3></td>\n");
#endif

	fprintf(table_file, "\t\t\t\t<td><tt><h3>Stack data pointer</tt></h3></td>\n");
	fprintf(table_file, "\t\t\t\t<td><tt><h3>Data</tt></h3></td>\n");

#ifdef CANARY_PROTECTION
	fprintf(table_file, "\t\t\t\t<td><tt><h3>Data canary2</tt></h3></td>\n");

	fprintf(table_file, "\t\t\t\t<td><tt><h3>Stack canary2</tt></h3></td>\n");
#endif

	fprintf(table_file, "\t\t\t</tr>\n");

	if(fclose(table_file))
		STACK_ERROR_CHECK(STACK_FILE_CLOSE_ERROR, stk);

	return STACK_NO_ERROR;
}

StackStatusCode HtmlTableLogFinisher(Stack_t* stk) {

	FILE* table_file = fopen(stk->log.files[TABLE].file_path, "a");
	if (!table_file)
		STACK_ERROR_CHECK(STACK_FILE_OPEN_ERROR, stk);

	fprintf(table_file, "\t\t</table>\n");

	if (fclose(table_file))
		STACK_ERROR_CHECK(STACK_FILE_CLOSE_ERROR, stk);

	return STACK_NO_ERROR;
}

StackStatusCode HtmlLogFinisher(Stack_t* stk) {

	StackStatusCode status = STACK_NO_ERROR;

	status = HtmlTableLogFinisher(stk);
	STACK_ERROR_CHECK(status, stk);

	for (size_t i = 0; i < stk->log.cnt_files; i++) {
		FILE* log_file = fopen(stk->log.files[i].file_path, "a");
		if (!log_file)
			STACK_ERROR_CHECK(STACK_FILE_OPEN_ERROR, stk);

		fprintf(log_file, "\t\t<p><a href='#top' name='down' class='anchor'><button class='btn'><tt>TOP</tt></button></a></p>\n");
		fprintf(log_file, "\t</body>\n");
		fprintf(log_file, "</html>\n");

		if (fclose(log_file))
			STACK_ERROR_CHECK(STACK_FILE_CLOSE_ERROR, stk);
	}

	RunMainHtmlFile(stk);

	return STACK_NO_ERROR;
}

StackStatusCode RunMainHtmlFile(Stack_t* stk) {

	StackStatusCode status = STACK_NO_ERROR;

	char* open_log_file = NULL;
	status = StrConcatenation("open ", stk->log.files[MAIN].file_path, &open_log_file, stk);
	STACK_ERROR_CHECK(status, stk);

	if(system(open_log_file))
		STACK_ERROR_CHECK(STACK_RUN_HTML_ERROR, stk);

#ifdef N_DEBUG
	printf("\n\n" GREEN("HTML FILE WAS RUN") "\n\n");
#endif

	if (open_log_file) {
		free(open_log_file);
		open_log_file = NULL;
	}

	return STACK_NO_ERROR;
}

StackStatusCode DoStackDumpMain(Stack_t* stk, DumpInfo dump_info) {

	static size_t number = 1;

	if (!stk)
		STACK_ERROR_CHECK(STACK_POINTER_ERROR, stk);

	FILE* main_file = fopen(stk->log.files[MAIN].file_path, "a");
	if (!main_file)
		STACK_ERROR_CHECK(STACK_FILE_OPEN_ERROR, stk);

	fprintf(main_file, "\t\t<div class='main_dump'>\n");

	fprintf(main_file, "\t\t\t<h3><p><tt>DUMP <span class='main_dump_number'>#%zu</span> called from <span class='stack_file_name'>%s</span>: "
					  "<span class='stack_line'>%zu</span> (after <span class='stack_function'>%s</span>)"
					  "</tt></p></h3>\n\t\t\t<tt><pre>\t{</pre></tt>\n",
					  number++, dump_info.file, dump_info.line, dump_info.func);

#ifdef HASH_PROTECTION
	fprintf(main_file, "\t\t\t<p><tt><pre>\t\t<span class='parameter'>stack hash</span> = <span class='hash'>%llx</span></pre></tt></p>\n",
					  stk->hash);
#endif

#ifdef CANARY_PROTECTION
	fprintf(main_file, "\t\t\t<p><tt><pre>\t\t<span class='parameter'>stack canary1</span> = <span class='hex_speak'>0x%llX</span></pre></tt></p>\n",
					  stk->canary1);
#endif

	fprintf(main_file, "\t\t\t<p><tt><pre>\t\t<span class='parameter'>capacity</span> = <span class='value'>%zu</span></pre></tt></p>\n",
					  stk->capacity);
	fprintf(main_file, "\t\t\t<p><tt><pre>\t\t<span class='parameter'>size</span> = <span class='value'>%zu</span></pre></tt></p>\n",
					  stk->size);

	if (stk->data) {

#ifdef HASH_PROTECTION
		fprintf(main_file, "\t\t\t<p><tt><pre>\t\t<span class='parameter'>data hash</span> = <span class='hash'>%llx</span></pre></tt></p>\n",
						  stk->data_hash);
#endif

#ifdef CANARY_PROTECTION
		fprintf(main_file, "\t\t\t<p><tt><pre>\t\t<span class='parameter'>data canary1</span> [<span class='address'>%p</span>] = "
						  "<span class='hex_speak'>0x%llX</span></pre></tt></p>\n",
						  (Canary_t*)((char*)stk->data - sizeof(Canary_t) - sizeof(Canary_t) % ALIGNMENT),
						  (*(Canary_t*)((char*)stk->data - sizeof(Canary_t) - sizeof(Canary_t) % ALIGNMENT)));
#endif

		fprintf(main_file, "\t\t\t<p><tt><pre>\t\t<span class='parameter'>data</span> [<span class='address'>%p</span>]:</pre></tt></p>\n"
						  "\t\t\t<p><tt><pre>\t\t\t{</pre></tt></p>\n", stk->data);
		for (size_t i = 0; i < stk->capacity; i++)
			(i < stk->size) ? fprintf(main_file, "\t\t\t<p><tt><pre>\t\t\t\t*<span class='parameter'>[%zu]</span> = "
												"<span class='value'>%lg</span></pre></tt></p>\n",
												i, *(stk->data + i)) :
							  fprintf(main_file, "\t\t\t<p><tt><pre>\t\t\t\t <span class='parameter'>[%zu]</span> = "
							  					"<span class='poison_value'>%lg</span> "
							  					"<span class='poison'><b>(POISON)</b></span></pre></tt></p>\n",
							  		  			i, *(stk->data + i));
		fprintf(main_file, "\t\t\t<tt><pre>\t\t\t}</pre></tt>\n");
	}

#ifdef CANARY_PROTECTION
	if (stk->data) {
		size_t data_size = stk->capacity * sizeof(Stack_elem_t);
		fprintf(main_file, "\t\t\t<p><tt><pre>\t\t<span class='parameter'>data canary2</span> [<span class='address'>%p</span>] = "
						  "<span class='hex_speak'>0x%llX</span></pre></tt></p>\n",
						  (Canary_t*)((char*)stk->data + stk->capacity * sizeof(Stack_elem_t)),
						  (*(Canary_t*)((char*)stk->data + data_size + (ALIGNMENT - data_size % ALIGNMENT))));
	}

	fprintf(main_file, "\t\t\t<p><tt><pre>\t\t<span class='parameter'>stack canary2</span> = <span class='hex_speak'>0x%llX</span></pre></tt></p>\n",
					  stk->canary2);
#endif

	fprintf(main_file, "\t\t\t<tt><pre>\t}</pre></tt>\n");

	fprintf(main_file, "\t\t</div>\n");

	if (fclose(main_file))
		STACK_ERROR_CHECK(STACK_FILE_CLOSE_ERROR, stk);

	return STACK_NO_ERROR;
}

StackStatusCode DoStackDumpTable(Stack_t* stk) {

	StackStatusCode status = STACK_NO_ERROR;

	STACK_VERIFY(stk);

	static size_t number = 1;

	FILE* table_file = fopen(stk->log.files[TABLE].file_path, "a");
	if (!table_file)
		STACK_ERROR_CHECK(STACK_FILE_OPEN_ERROR, stk);

	fprintf(table_file, "\t\t\t<tr>\n");
	fprintf(table_file, "\t\t\t\t<td class = 'stack_dump_number'><tt>%zu</tt></td>\n", number++);

#ifdef HASH_PROTECTION
	fprintf(table_file, "\t\t\t\t<td class = 'hash'><tt>%llX</tt></td>\n", stk->hash);
#endif

#ifdef CANARY_PROTECTION
	fprintf(table_file, "\t\t\t\t<td class = 'hex_speak'><tt>%llX</tt></td>\n", stk->canary1);
#endif

	fprintf(table_file, "\t\t\t\t<td class = 'stack_capacity'><tt>%zu</tt></td>\n", stk->capacity);
	fprintf(table_file, "\t\t\t\t<td class = 'stack_size'><tt>%zu</tt></td>\n", stk->size);

#ifdef HASH_PROTECTION
	fprintf(table_file, "\t\t\t\t<td class = 'hash'><tt>%llX</tt></td>\n", stk->data_hash);
#endif

#ifdef CANARY_PROTECTION
	fprintf(table_file, "\t\t\t\t<td class = 'hex_speak'><tt>%llX</tt></td>\n",
			(*(Canary_t*)((char*)stk->data - sizeof(Canary_t) - sizeof(Canary_t) % ALIGNMENT)));
#endif

	fprintf(table_file, "\t\t\t\t<td class = 'stack_data_pointer'><tt>%p</tt></td>\n", stk->data);

	fprintf(table_file, "\t\t\t\t<td class = 'stack_data'><tt>");
	for (size_t i = 0; i < stk->size; i++)
		fprintf(table_file, "%lg ", *(stk->data + i));
	fprintf(table_file, "</tt></td>\n");

#ifdef CANARY_PROTECTION
	size_t data_size = stk->capacity * sizeof(Stack_elem_t);
	fprintf(table_file, "\t\t\t\t<td class = 'hex_speak'><tt>%llX</tt></td>\n",
			(*(Canary_t*)((char*)stk->data + data_size + (ALIGNMENT - data_size & ALIGNMENT))));

	fprintf(table_file, "\t\t\t\t<td class = 'hex_speak'><tt>%llX</tt></td>\n", stk->canary2);
#endif

	fprintf(table_file, "\t\t\t</tr>\n");

	if (fclose(table_file))
		STACK_ERROR_CHECK(STACK_FILE_CLOSE_ERROR, stk);

	return STACK_NO_ERROR;
}
