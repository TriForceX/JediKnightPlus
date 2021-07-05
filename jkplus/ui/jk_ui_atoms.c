/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2020
=====================================================================
[Description]: UI building blocks and support functions
=====================================================================
*/

#include "../../code/ui/ui_local.h"	// Original header

/*
=====================================================================
Inspect file function
=====================================================================
*/
static int JKMod_UI_InspectFile(char *filename)
{
	char *name;
	char *q, *r, *buf;
	int len;
	fileHandle_t	f;

	if ((name = BG_TempAlloc(256)) == 0) // Assume the worst
	{ 
		return 1;
	}

	Q_strncpyz(name, filename, 255);
	len = trap_FS_FOpenFile(name, &f, FS_READ);
	if (!f)
	{ 
		// No file, try extension
		Q_strcat(name, 255, ".cfg");
		len = trap_FS_FOpenFile(name, &f, FS_READ);
		if (!f)
		{ 
			// No file, assume the worst
			return 1;
		}
	}
	// Read file into buffer
	if ((buf = BG_TempAlloc(len + 1)) == 0)
	{
		trap_FS_FCloseFile(f);
		return 1; // Assume the worst
	}
	trap_FS_Read(buf, len, f);
	trap_FS_FCloseFile(f);

	// Look for illegal commands
	if (strstr(buf, "+moveup") || strstr(buf, "+attack") ||
		strstr(buf, "+forward") || strstr(buf, "+back") ||
		strstr(buf, "+movedown"))
	{
		BG_TempFree(len + 257); // Free the temp buffer
		return 1;
	}

	// Look for exec commands
	q = buf;
	while (*q)
	{
		while (*q && (*q == ';' || *q == ' ' || *q == '\t' || *q == '\n' || *q == '\r')) q++;  // Skip leading ';' or white space
		if (*q && (!Q_stricmpn(q, "exec ", 5) || !Q_stricmpn(q, "exec\t", 5)))
		{ 
			// This is an exec command, snag the filename
			r = name;
			q += 4; // Skip the exec
			while (*q == ' ' || *q == '\t') q++;  // Skip white space
			if (*q == '\"')
			{ 
				// This next token is quoted
				q++;
				while (*q && *q != '\"' && *q != '\n' && *q != '\r') *r++ = *q++;
				if (*q && *q == '\"') q++;	// Eat the last quote, if there
				*r = 0;						// Terminate quoted string
			}
			else
			{
				while (*q && *q != ';' && *q != ' ' && *q != '\t' && *q != '\n' && *q != '\r')
				{
					*r++ = *q++;
				}
				*r = 0; // Terminate filename string
			}
			// Recurse
			if (JKMod_UI_InspectFile(name) == 1)
			{
				BG_TempFree(len + 257); // Free the temp buffer
				return 1;
			}
		}
		while (*q && *q != ';' && *q != '\n' && *q != '\r')
		{
			if (*q == '\"')
			{
				q++;
				while (*q && *q != '\"' && *q != '\n' && *q != '\r') q++;
				if (*q && *q == '\"') q++; // Eat the last quote, if there
			}
			else
			{
				q++;
			}
		} 
		// Look for the next command in the string
	}

	// Look for vstr commands
	q = buf;
	while (*q)
	{
		while (*q && (*q == ';' || *q == ' ' || *q == '\t' || *q == '\n' || *q == '\r')) q++;  // Skip leading ';' or white space
		if (*q && (!Q_stricmpn(q, "vstr ", 5) || !Q_stricmpn(q, "vstr\t", 5)))
		{ 
			// This is an vstr command, snag the cvar name
			r = name;
			q += 4; // Skip the vstr
			while (*q == ' ' || *q == '\t') q++;  // Skip white space
			if (*q == '\"')
			{ 
				//This next token is quoted
				q++;
				while (*q && *q != '\"' && *q != '\n' && *q != '\r') *r++ = *q++;
				if (*q && *q == '\"') q++;	// Eat the last quote, if there
				*r = 0;						// Terminate quoted string
			}
			else
			{
				while (*q && *q != ';' && *q != ' ' && *q != '\t' && *q != '\n' && *q != '\r')
				{
					*r++ = *q++;
				}
				*r = 0; // Terminate cvar name string
			}
			// Recurse
			if (JKMod_UI_InspectFile(name) == 1)
			{
				BG_TempFree(len + 257); // Free the temp buffer
				return 1;
			}
		}
		while (*q && *q != ';' && *q != '\n' && *q != '\r')
		{
			if (*q == '\"')
			{
				q++;
				while (*q && *q != '\"' && *q != '\n' && *q != '\r') q++;
				if (*q && *q == '\"') q++; // Eat the last quote, if there
			}
			else
			{
				q++;
			}
		} 
		// Look for the next command in the string
	}

	// All OK, free the temp buffer
	BG_TempFree(len + 257);
	return 0;
}

/*
=====================================================================
Inspect cvar function
=====================================================================
*/
static int JKMod_UI_InspectCvar(char *cvarname)
{
	char *buf, *name;
	char *q, *r;

	if ((buf = BG_TempAlloc(256)) == 0) // Assume the worst
	{ 
		return 1;
	}
	if ((name = BG_TempAlloc(256)) == 0) // Assume the worst
	{ 
		return 1;
	}

	// Get the value of the cvar	
	trap_Cvar_VariableStringBuffer(cvarname, buf, 255);

	// Look for illegal commands
	if (strstr(buf, "+moveup") || strstr(buf, "+attack") ||
		strstr(buf, "+forward") || strstr(buf, "+back") ||
		strstr(buf, "+movedown"))
	{
		BG_TempFree(512); // Free the temp buffer
		return 1;
	}

	// Look for exec commands
	q = buf;
	while (*q)
	{
		while (*q && (*q == ';' || *q == ' ' || *q == '\t')) q++;  // Skip leading ';' or white space
		if (*q && (!Q_stricmpn(q, "exec ", 5) || !Q_stricmpn(q, "exec\t", 5)))
		{ 
			// This is an exec command, snag the filename
			r = name;
			q += 4; //skip the exec
			while (*q == ' ' || *q == '\t') q++;  // Skip white space
			if (*q == '\"')
			{ 
				// This next token is quoted
				q++;
				while (*q && *q != '\"') *r++ = *q++;
				if (*q) q++;	// Eat the last quote
				*r = 0;			// Terminate quotation string
			}
			else
			{
				while (*q && *q != ';' && *q != ' ' && *q != '\t')
				{
					*r++ = *q++;
				}
				*r = 0; // Terminate filename string
			}
			// Recurse
			if (JKMod_UI_InspectFile(name) == 1)
			{
				BG_TempFree(512); // Free the temp buffer
				return 1;
			}
		}
		while (*q && *q != ';')
		{
			if (*q == '\"')
			{
				q++;
				while (*q && *q != '\"') q++;
				if (*q) q++; // Eat the last quote
			}
			else
			{
				q++;
			}
		}
		// Look for the next command in the string
	}

	// We don't actually want to recurse here (like in the file case), since the execution of vstr is not recursive!
	// All OK, free the temp buffer
	BG_TempFree(512);
	return 0;
}

/*
=====================================================================
Macro scan function
=====================================================================
*/
void JKMod_UI_MacroScan(void)
{
	// Process binds and scan for macros
	int i, j, macro;
	char buf[256];

	// Com_Printf("starting macro scan\n");
	for (i = 0; i < MAX_KEYS; i++)
	{
		trap_Key_GetBindingBuf(i, buf, 256);
		buf[255] = 0;	// Just in case
		macro = 0;		// Assume it is OK until otherwise

		if (strchr(buf, ';'))
		{ 
			// Potential illegal macro
			char name[255];
			char *q, *r;

			if (strstr(buf, "+moveup")) macro = 1;
			else if (strstr(buf, "+attack")) macro = 1;
			else if (strstr(buf, "+forward")) macro = 1;
			else if (strstr(buf, "+back")) macro = 1;
			else if (strstr(buf, "+movedown")) macro = 1;
			else
			{
				// Now look for exec statements
				q = buf;
				while (*q)
				{
					while (*q && (*q == ';' || *q == ' ' || *q == '\t')) q++;  // Skip leading ';' or white space
					if (*q && (!Q_stricmpn(q, "exec ", 5) || !Q_stricmpn(q, "exec\t", 5)))
					{ 
						// This is an exec command, snag the filename
						r = name;
						q += 4; // Skip the exec
						while (*q == ' ' || *q == '\t') q++;  // Skip white space
						if (*q == '\"')
						{ 
							// This next token is quoted
							q++;
							while (*q && *q != '\"' && *q != '\n' && *q != '\r') *r++ = *q++;
							if (*q) q++;	// Eat the last quote or white space char
							*r = 0;			// Terminate quotation string
						}
						else
						{
							while (*q && *q != ';' && *q != ' ' && *q != '\t')
							{
								*r++ = *q++;
							}
							*r = 0; // Terminate filename string
						}
						if (JKMod_UI_InspectFile(name) == 1)
						{
							macro = 1;
							break;
						}
					}
					while (*q && *q != ';')
					{
						if (*q == '\"')
						{
							q++;
							while (*q && *q != '\"') q++;
							if (*q) q++; // Eat the last quote
						}
						else
						{
							q++;
						}
					} 
					// Look for the next command in the string
				}

				// Now look for vstr commands
				q = buf;
				while (*q)
				{
					while (*q && (*q == ';' || *q == ' ' || *q == '\t')) q++;  // Skip leading ';' or white space
					if (*q && (!Q_stricmpn(q, "vstr ", 5) || !Q_stricmpn(q, "vstr\t", 5)))
					{ 
						// This is an vstr command, snag the cvar name
						r = name;
						q += 4; // Skip the vstr
						while (*q == ' ' || *q == '\t') q++;  // Skip white space
						if (*q == '\"')
						{ 
							// This next token is quoted
							q++;
							while (*q && *q != '\"') *r++ = *q++;
							if (*q) q++;	// Eat the last quote
							*r = 0;			// Terminate quoted string
						}
						else
						{
							while (*q && *q != ';' && *q != ' ' && *q != '\t')
							{
								*r++ = *q++;
							}
							*r = 0; // Terminate cvar name string
						}
						if (JKMod_UI_InspectCvar(name) == 1)
						{
							macro = 1;
							break;
						}
					}

					while (*q && *q != ';')
					{
						if (*q == '\"')
						{
							q++;
							while (*q && *q != '\"') q++;
							if (*q) q++; // Eat the last quote
						}
						else
						{
							q++;
						}
					} 
					// Look for the next command in the string
				}
			}
		}
		else if (!Q_stricmpn(buf, "exec ", 5) || !Q_stricmpn(buf, "exec\t", 5))
		{ 
			// Keys is bound to exec scripts in files, lets look at the file
			char fname[255];
			char *q, *r;

			q = buf + 4;
			// Snag the filename
			r = fname;
			while (*q == ' ' || *q == '\t') q++;  // Skip white space
			if (*q == '\"')
			{ // This next token is quoted
				q++;
				while (*q && *q != '\"') *r++ = *q++;
				if (*q) q++;	// Eat the last quote
				*r = 0;			// Terminate quoted string
			}
			else
			{
				while (*q && *q != ';' && *q != ' ' && *q != '\t')
				{
					*r++ = *q++;
				}
				*r = 0; // Terminate filename string
			}
			if (JKMod_UI_InspectFile(fname) == 1) macro = 1;
		}
		else if (!Q_stricmpn(buf, "vstr ", 5) || !Q_stricmpn(buf, "vstr\t", 5))
		{ 
			// Key is bound to an evaluation of a variable, lets look at the variable
			char cname[255];
			char *q, *r;

			q = buf + 4;
			// Snag the cvariable to be evaluated
			r = cname;
			while (*q == ' ' || *q == '\t') q++; // Skip white space
			if (*q == '\"')
			{ 
				// This next token is quoted
				q++;
				while (*q && *q != '\"') *r++ = *q++;
				if (*q) q++;	// Eat the last quote
				*r = 0;			// Terminate quoted string
			}
			else
			{
				while (*q &&  *q != ';' && *q != ' ' && *q != '\t')
				{
					*r++ = *q++;
				}
				*r = 0; // Terminate cvar name string
			}
			if (JKMod_UI_InspectCvar(cname) == 1) macro = 1;
		}

		if (macro == 1)
		{ 
			// Found illegal macro
			Com_Printf("^3Illegal binding found... removing\n");

			for (j = 0; j < strlen(buf); j++)
			{ 
				// Replace ';' with ':'
				if (buf[j] == ';') buf[j] = ':';
			}
			trap_Key_SetBinding(i, va("echo ^3Illegal macro: ^7%s", buf));
			trap_Cmd_ExecuteText(EXEC_APPEND, va("macroused\n"));
		}
	}
}

/*
=====================================================================
Macro enable function
=====================================================================
*/
void JKMod_UI_MacroEnable(void) 
{
	// Process binds and scan for macros, the re-enable them
	int		i, j;
	char	buf[256];
	char	*find = "echo ^3Illegal macro: ^7";
	int		findSize = strlen(find);
	
	// Com_Printf("Starting macro enable scan\n");
	for (i = 0; i < MAX_KEYS; i++)
	{
		trap_Key_GetBindingBuf(i, buf, 256);
		buf[255] = 0; // Just in case
		if (!Q_stricmpn(buf, find, findSize))
		{ 
			// Found a commented macro
			for (j = findSize; j < strlen(buf); j++)
			{ 
				// Replace ':' with ';'
				if (buf[j] == ':') buf[j] = ';';
			}
			trap_Key_SetBinding(i, buf + findSize);
		}
	}
}

/*
=====================================================================
Console command function
=====================================================================
*/
qboolean JKMod_UI_ConsoleCommand(char *cmd)
{
	// Scan command
	if (Q_stricmp(cmd, "jk_ui_cmd_macroScan") == 0)
	{
		JKMod_UI_MacroScan();
		return qtrue;
	}
	// Enable command
	if (Q_stricmp(cmd, "jk_ui_cmd_macroEnable") == 0)
	{
		JKMod_UI_MacroEnable();
		return qtrue;
	}
	// Open client options pop-up
	if (Q_stricmp(cmd, "jk_ui_cmd_clientPopUp") == 0)
	{
		if (!(trap_Key_GetCatcher() & KEYCATCH_UI)) 
		{
			trap_Cvar_Set("cl_paused", "1");
			trap_Key_SetCatcher(KEYCATCH_UI);
			Menus_ActivateByName("ingame_jkmod_popup");
		}
		return qtrue;
	}

	return qfalse;
}
