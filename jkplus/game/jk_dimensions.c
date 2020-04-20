/*
======================= Jedi Knight Plus Mod ========================
By Tr!Force. Work copyrighted (C) with holder attribution 2005 - 2020
=====================================================================
[Description]: Main dimension stuff...
=====================================================================
*/

#include "../../code/game/g_local.h" // Main header

// Dimension entity save
typedef struct dimension_save_s
{
	int entityNum;
	int contents;

} dimension_save_t;

dimension_save_t dimension_save[MAX_GENTITIES];

/*
=====================================================================
Dimensions trace
=====================================================================
*/
void JKPlus_Dimensions(trace_t *results, const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end, int passEntityNum, int contentmask) 
{
	static int i, j; // This gets called a lot, make it static
	static int reset;
	static int done;

	done = 0;
	j = 0; // Count number of entities in the save array

	// Dealing with a client (Consider passEntities that are clients)
	if (passEntityNum >= 0 && passEntityNum < MAX_CLIENTS)
	{
		if ((jkcvar_altDimensions.integer & (1 << DIMENSION_CHAT)) && g_entities[passEntityNum].client && (g_entities[passEntityNum].client->ps.eFlags & JK_CHAT_PROTECT))
		{
			// Client is in a dimension where no FFA fighting is allowed
			for (i = 0; i < MAX_CLIENTS; i++)
			{ 
				// Save contents and change as appropriate
				if (i == passEntityNum) continue;
				if (g_entities[i].inuse && g_entities[i].client) 
				{
					if (g_entities[i].r.contents)
					{ 
						// There are some strange situations where contents can get over-written twice... sigh
						dimension_save[j].entityNum = i;
						dimension_save[j].contents = g_entities[i].r.contents;
						j++;
						g_entities[i].r.contents = 0;
					}
					if (g_entities[i].client->ps.saberEntityNum)
					{ 
						// Set their lightsaber content too (If they have one)
						if (g_entities[g_entities[i].client->ps.saberEntityNum].r.contents)
						{ 
							// There are some strange situations where contents can get over-written twice... sigh
							dimension_save[j].entityNum = g_entities[i].client->ps.saberEntityNum;
							dimension_save[j].contents = g_entities[g_entities[i].client->ps.saberEntityNum].r.contents;
							j++;
							g_entities[g_entities[i].client->ps.saberEntityNum].r.contents = 0;
						}
					}
				}
			}
		}
		// passEntityNum is dueling
		else if ((jkcvar_altDimensions.integer & (1 << DIMENSION_DUEL)) && g_entities[passEntityNum].client && g_entities[passEntityNum].client->ps.duelInProgress)
		{ 
			for (i = 0; i < MAX_CLIENTS; i++)
			{ 
				// Save contents and change as appropriate
				if (i == passEntityNum) continue;
				if (i == g_entities[passEntityNum].client->ps.duelIndex) continue; // Skip player he is dueling too!
				if (g_entities[i].inuse && g_entities[i].client) 
				{
					if (g_entities[i].r.contents)
					{ 
						// There are some strange situations where contents can get over-written twice... sigh						
						dimension_save[j].entityNum = i;
						dimension_save[j].contents = g_entities[i].r.contents;
						j++;
						g_entities[i].r.contents = 0;
					}
					if (g_entities[i].client->ps.saberEntityNum)
					{ 
						// Set their lightsaber content too (If they have one)
						if (g_entities[g_entities[i].client->ps.saberEntityNum].r.contents)
						{ 
							// There are some strange situations where contents can get over-written twice... sigh						
							dimension_save[j].entityNum = g_entities[i].client->ps.saberEntityNum;
							dimension_save[j].contents = g_entities[g_entities[i].client->ps.saberEntityNum].r.contents;
							j++;
							g_entities[g_entities[i].client->ps.saberEntityNum].r.contents = 0;
						}
					}
				}
			}
		}
	}
	// Dealing with a lightsaber 
	else if (g_entities[passEntityNum].classname && !strcmp(g_entities[passEntityNum].classname, "lightsaber"))
	{
		if ((jkcvar_altDimensions.integer & (1 << DIMENSION_CHAT)) && g_entities[g_entities[passEntityNum].r.ownerNum].client && (g_entities[g_entities[passEntityNum].r.ownerNum].client->ps.eFlags & JK_CHAT_PROTECT))
		{ 
			// Lightsaber is in a dimension where no FFA is allowed
			for (i = 0; i < MAX_CLIENTS; i++)
			{ 
				// Save contents and change as appropriate
				if (i == g_entities[passEntityNum].r.ownerNum) continue; // Skip owner of saber
				if (g_entities[i].inuse && g_entities[i].client) 
				{
					if (g_entities[i].r.contents)
					{ 
						// There are some strange situations where contents can get over-written twice... sigh						
						dimension_save[j].entityNum = i;
						dimension_save[j].contents = g_entities[i].r.contents;
						j++;
						g_entities[i].r.contents = 0;
					}
					if (g_entities[i].client->ps.saberEntityNum)
					{ 
						// Set their lightsaber content too (If they have one)
						if (g_entities[g_entities[i].client->ps.saberEntityNum].r.contents)
						{ 
							// There are some strange situations where contents can get over-written twice... sigh						
							dimension_save[j].entityNum = g_entities[i].client->ps.saberEntityNum;
							dimension_save[j].contents = g_entities[g_entities[i].client->ps.saberEntityNum].r.contents;
							j++;
							g_entities[g_entities[i].client->ps.saberEntityNum].r.contents = 0;
						}
					}
				}
			}
		}
		else if ((jkcvar_altDimensions.integer & (1 << DIMENSION_DUEL)) && g_entities[g_entities[passEntityNum].r.ownerNum].client && g_entities[g_entities[passEntityNum].r.ownerNum].client->ps.duelInProgress)
		{	
			// Owner of the lightsaber is in a duel
			for (i = 0; i < MAX_CLIENTS; i++)
			{ 
				// Save contents and change as appropriate
				if (i == g_entities[passEntityNum].r.ownerNum) continue; // Skip owner of saber
				if (i == g_entities[g_entities[passEntityNum].r.ownerNum].client->ps.duelIndex) continue; // Skip player he is dueling too
				if (g_entities[i].inuse && g_entities[i].client)
				{
					if (g_entities[i].r.contents)
					{ 
						// There are some strange situations where contents can get over-written twice... sigh						
						dimension_save[j].entityNum = i;
						dimension_save[j].contents = g_entities[i].r.contents;
						j++;
						g_entities[i].r.contents = 0;
					}
					if (g_entities[i].client->ps.saberEntityNum)
					{ 
						// Set their lightsaber content too (If they have one)
						if (g_entities[g_entities[i].client->ps.saberEntityNum].r.contents)
						{ 
							// There are some strange situations where contents can get over-written twice... sigh						
							dimension_save[j].entityNum = g_entities[i].client->ps.saberEntityNum;
							dimension_save[j].contents = g_entities[g_entities[i].client->ps.saberEntityNum].r.contents;
							j++;
							g_entities[g_entities[i].client->ps.saberEntityNum].r.contents = 0;
						}
					}
				}
			}
		}
	}

	trap_Trace(results, start, mins, maxs, end, passEntityNum, contentmask);

	// Consider results
	while (results->fraction < 1.0)
	{
		if (results->entityNum >= 0 && results->entityNum < MAX_CLIENTS)
		{ 
			// Hit entity is a client
			if ((jkcvar_altDimensions.integer & (1 << DIMENSION_CHAT)) && (g_entities[results->entityNum].client->ps.eFlags & JK_CHAT_PROTECT))
			{	
				// Hit a client in a dimension where no FFA fighting is allowed
				if (g_entities[results->entityNum].r.contents)
				{ 
					// There are some strange situations where contents can get over-written twice... sigh						
					dimension_save[j].entityNum = results->entityNum;
					dimension_save[j].contents = g_entities[results->entityNum].r.contents;
					j++;
					g_entities[results->entityNum].r.contents = 0; // Change contents of hit entity and run trace again
				}
				if (g_entities[results->entityNum].client->ps.saberEntityNum)
				{ 
					// Set their lightsaber content too (If they have one)
					if (g_entities[g_entities[results->entityNum].client->ps.saberEntityNum].r.contents)
					{ 
						// There are some strange situations where contents can get over-written twice... sigh						
						dimension_save[j].entityNum = g_entities[results->entityNum].client->ps.saberEntityNum;
						dimension_save[j].contents = g_entities[g_entities[results->entityNum].client->ps.saberEntityNum].r.contents;
						j++;
						g_entities[g_entities[results->entityNum].client->ps.saberEntityNum].r.contents = 0;
					}
				}
			}
			else if ((jkcvar_altDimensions.integer & (1 << DIMENSION_DUEL)) && g_entities[results->entityNum].client->ps.duelInProgress)
			{ 
				// Hit a client currently dueling (If a client then your duel index should equal the results entitynum)
				if (g_entities[passEntityNum].client && g_entities[passEntityNum].client->ps.duelInProgress && g_entities[passEntityNum].client->ps.duelIndex == results->entityNum)
				{ 
					// You have hit the person you are dueling with so...
					done = 1;
				}
				// If a lightsaber then your owners duel index shout be the results entitynum
				else if (g_entities[passEntityNum].classname && !strcmp(g_entities[passEntityNum].classname, "lightsaber") && 
						g_entities[g_entities[passEntityNum].r.ownerNum].client->ps.duelInProgress && 
						g_entities[g_entities[passEntityNum].r.ownerNum].client->ps.duelIndex == results->entityNum)
				{ 
					// Your saber has hit the person you are dueling with so...
					done = 1;
				}
				else if (g_entities[passEntityNum].classname && !strcmp(g_entities[passEntityNum].classname, "laserTrap") && 
						g_entities[g_entities[passEntityNum].r.ownerNum].client->ps.duelInProgress && 
						(g_entities[g_entities[passEntityNum].r.ownerNum].client->ps.duelIndex == results->entityNum || 
						g_entities[passEntityNum].r.ownerNum == results->entityNum))
				{ 
					// Your laser trap has hit your opponent or you!
					done = 1;
				}
				else
				{
					// Otherwise
					if (g_entities[results->entityNum].r.contents)
					{ 
						// There are some strange situations where contents can get over-written twice... sigh						
						dimension_save[j].entityNum = results->entityNum;
						dimension_save[j].contents = g_entities[results->entityNum].r.contents;
						j++;
						g_entities[results->entityNum].r.contents = 0; // Change contents of hit entity and run trace again
					}
					if (g_entities[results->entityNum].client->ps.saberEntityNum)
					{ 
						// Set their lightsaber content too (If they have one)
						if (g_entities[g_entities[results->entityNum].client->ps.saberEntityNum].r.contents)
						{ 
							// There are some strange situations where contents can get over-written twice... sigh						
							dimension_save[j].entityNum = g_entities[results->entityNum].client->ps.saberEntityNum;
							dimension_save[j].contents = g_entities[g_entities[results->entityNum].client->ps.saberEntityNum].r.contents;
							j++;
							g_entities[g_entities[results->entityNum].client->ps.saberEntityNum].r.contents = 0;
						}
					}
				}
			}
			else
			{
				// Hit client is not part of a dimension
				done = 1; 
			}
		}
		// Hit entity is a lightsaber
		else if (g_entities[results->entityNum].classname && !strcmp(g_entities[results->entityNum].classname, "lightsaber"))
		{ 
			if ((jkcvar_altDimensions.integer & (1 << DIMENSION_CHAT)) && g_entities[g_entities[results->entityNum].r.ownerNum].client && (g_entities[g_entities[results->entityNum].r.ownerNum].client->ps.eFlags & JK_CHAT_PROTECT))
			{ 
				// It is owned by someone in a dimension where no FFA is allowed
				if (g_entities[g_entities[results->entityNum].r.ownerNum].r.contents)
				{ 
					// There are some strange situations where contents can get over-written twice... sigh						
					dimension_save[j].entityNum = g_entities[results->entityNum].r.ownerNum;
					dimension_save[j].contents = g_entities[g_entities[results->entityNum].r.ownerNum].r.contents;
					j++;
					g_entities[g_entities[results->entityNum].r.ownerNum].r.contents = 0; // Set saber owner contents to zero
				}
				if (g_entities[results->entityNum].r.contents)
				{ 
					// There are some strange situations where contents can get over-written twice... sigh
					dimension_save[j].entityNum = results->entityNum;
					dimension_save[j].contents = g_entities[results->entityNum].r.contents;
					j++;
					g_entities[results->entityNum].r.contents = 0; // Set the lightsaber contents to zero, run the trace again
				}
			}
			else if ((jkcvar_altDimensions.integer & (1 << DIMENSION_DUEL)) && g_entities[g_entities[results->entityNum].r.ownerNum].client && g_entities[g_entities[results->entityNum].r.ownerNum].client->ps.duelInProgress)
			{ 
				// It is owned by someone in a duel (If a client then your duel index should equal the owner of the results entitynum)
				if (g_entities[passEntityNum].client && 
					g_entities[passEntityNum].client->ps.duelInProgress && 
					g_entities[passEntityNum].client->ps.duelIndex == g_entities[results->entityNum].r.ownerNum)
				{ 
					// You have hit the saber of a person you are dueling with so...
					done = 1;
				}
				// If not a client then your owners duel index should be the owner of the results entitynum
				else if (g_entities[g_entities[passEntityNum].r.ownerNum].client && 
						g_entities[g_entities[passEntityNum].r.ownerNum].client->ps.duelInProgress && 
						g_entities[g_entities[passEntityNum].r.ownerNum].client->ps.duelIndex == g_entities[results->entityNum].r.ownerNum)
				{ 
					// Something of yours (Eg. saber, missile) has hit the saber of a person you are dueling with so...
					done = 1;
				}
				else
				{
					// Otherwise
					if (g_entities[g_entities[results->entityNum].r.ownerNum].r.contents)
					{ 
						// There are some strange situations where contents can get over-written twice... sigh
						dimension_save[j].entityNum = g_entities[results->entityNum].r.ownerNum;
						dimension_save[j].contents = g_entities[g_entities[results->entityNum].r.ownerNum].r.contents;
						j++;
						g_entities[g_entities[results->entityNum].r.ownerNum].r.contents = 0; // Set saber owner contents to zero
					}
					if (g_entities[results->entityNum].r.contents)
					{ 
						// There are some strange situations where contents can get over-written twice... sigh
						dimension_save[j].entityNum = results->entityNum;
						dimension_save[j].contents = g_entities[results->entityNum].r.contents;
						j++;
						g_entities[results->entityNum].r.contents = 0; // Set the lightsaber contents to zero, run the trace again
					}
				}
			}
			else
			{
				// Hit lightsaber is not part of a dimension
				done = 1;
			}
		}
		// Hit entity is a sentryGun
		else if (g_entities[results->entityNum].classname && !strcmp(g_entities[results->entityNum].classname, "sentryGun"))
		{ 
			// we use 's' data here rather than 'r' data. First, the r data is not set up for the sentry gun when the
			// entity is created. Second, if we set it up, the behaviour of the trace call is changed. This is because the
			// trace call will check owner info in 'r' and not trace againsts things owned by passEntityNum
			if ((jkcvar_altDimensions.integer & (1 << DIMENSION_DUEL)) && g_entities[g_entities[results->entityNum].s.owner].client && g_entities[g_entities[results->entityNum].s.owner].client->ps.duelInProgress)
			{ 
				// Sentry gun is owned by someone in a duel (If a client then consider if the sentry gun is yours or your opponents)
				if (g_entities[passEntityNum].client && 
					g_entities[passEntityNum].client->ps.duelInProgress && 
					(g_entities[passEntityNum].client->ps.duelIndex == g_entities[results->entityNum].s.owner || 
					passEntityNum == g_entities[results->entityNum].s.owner))
				{ 
					// You have hit the sentry gun of a person you are dueling with so...
					done = 1;
				}
				// If not a client then your owners duel index should be the owner of the results entitynum
				else if (g_entities[g_entities[passEntityNum].r.ownerNum].client && 
						g_entities[g_entities[passEntityNum].r.ownerNum].client->ps.duelInProgress && 
						(g_entities[g_entities[passEntityNum].r.ownerNum].client->ps.duelIndex == g_entities[results->entityNum].s.owner || 
						g_entities[passEntityNum].r.ownerNum == g_entities[results->entityNum].s.owner))
				{ 
					// Something of yours (Eg. saber, missile) has hit your sentry gun or the sentry gun of a person you are dueling with so...
					done = 1;
				}
				else
				{
					// Otherwise
					if (g_entities[results->entityNum].r.contents)
					{ 
						// There are some strange situations where contents can get over-written twice... sigh						
						dimension_save[j].entityNum = results->entityNum;
						dimension_save[j].contents = g_entities[results->entityNum].r.contents;
						j++;
						g_entities[results->entityNum].r.contents = 0; // Set sentry gun owner contents to zero
					}
				}
			}
			else if ((jkcvar_altDimensions.integer & (1 << DIMENSION_DUEL)) && g_entities[passEntityNum].client && g_entities[passEntityNum].client->ps.duelInProgress)
			{ 
				// And we have to deal with one other case: if *you* are dueling and we did not hit above
				// because the owner of the sentry gun is not dueling, we want to skip this entitiy...
				// When you are dueling we try to set up contents before the trace, but we only change
				// the contents of clients and lightsabers, so it is still possilbe you could hit a sentry gun
				if (g_entities[results->entityNum].r.contents)
				{ 
					// There are some strange situations where contents can get over-written twice... sigh						
					dimension_save[j].entityNum = results->entityNum;
					dimension_save[j].contents = g_entities[results->entityNum].r.contents;
					j++;
					g_entities[results->entityNum].r.contents = 0; // Set sentry gun owner contents to zero
				}
			}
			else
			{
				// Hit sentry gun is not part of a dimension
				done = 1;
			}
		}
		// Hit entity is a portable shield
		else if (g_entities[results->entityNum].classname && !strcmp(g_entities[results->entityNum].classname, "item_shield"))
		{ 
			// We use 's' data here rather than 'r' data. First, the r data is not set up for the shield when the
			// entity is created. Second, if we set it up, the behaviour of the trace call is changed. This is because the
			// trace call will check owner info in 'r' and not trace againsts things owned by passEntityNum
			if ((jkcvar_altDimensions.integer & (1 << DIMENSION_DUEL)) && g_entities[g_entities[results->entityNum].s.owner].client && g_entities[g_entities[results->entityNum].s.owner].client->ps.duelInProgress)
			{ 
				// Shield is owned by someone in a duel (If a client then consider if the portable shield is yours or your opponents)
				if (g_entities[passEntityNum].client && 
					g_entities[passEntityNum].client->ps.duelInProgress && 
					(g_entities[passEntityNum].client->ps.duelIndex == g_entities[results->entityNum].s.owner ||
					passEntityNum == g_entities[results->entityNum].s.owner))
				{ 
					// You have hit the shield of a person you are dueling with so...
					done = 1;
				}
				// If not a client then your owners duel index should be the owner of the results entitynum
				else if (g_entities[g_entities[passEntityNum].r.ownerNum].client && 
						g_entities[g_entities[passEntityNum].r.ownerNum].client->ps.duelInProgress && 
						(g_entities[g_entities[passEntityNum].r.ownerNum].client->ps.duelIndex == g_entities[results->entityNum].s.owner ||
						g_entities[passEntityNum].r.ownerNum == g_entities[results->entityNum].s.owner))
				{ 
					// Something of yours (Eg. saber, missile) has hit your shield or the shield of a person you are dueling with so...
					done = 1;
				}
				else
				{
					// Otherwise
					if (g_entities[results->entityNum].r.contents)
					{ 
						// There are some strange situations where contents can get over-written twice... sigh						
						dimension_save[j].entityNum = results->entityNum;
						dimension_save[j].contents = g_entities[results->entityNum].r.contents;
						j++;
						g_entities[results->entityNum].r.contents = 0; // Set portable shield owner contents to zero
					}
				}
			}
			else if ((jkcvar_altDimensions.integer & (1 << DIMENSION_DUEL)) && g_entities[passEntityNum].client && g_entities[passEntityNum].client->ps.duelInProgress)
			{ 
				// And we have to deal with one other case: if *you* are dueling and we did not hit above
				// because the owner of the shield is not dueling, we want to skip this entitiy...
				// When you are dueling we try to set up contents before the trace, but we only change
				// the contents of clients and lightsabers, so it is still possilbe you could hit a sheild
				if (g_entities[results->entityNum].r.contents)
				{ 
					// There are some strange situations where contents can get over-written twice... sigh						
					dimension_save[j].entityNum = results->entityNum;
					dimension_save[j].contents = g_entities[results->entityNum].r.contents;
					j++;
					g_entities[results->entityNum].r.contents = 0; // Set portable shield owner contents to zero
				}
			}
			else
			{
				// Hit portable shield is not part of a dimension
				done = 1;
			}
		}
		else
		{
			// Hit entity is not a client or lightsaber or other, so proceed...
			done = 1; 
		}

		// Last check
		if (done)
		{
			break;
		}

		// Do trace
		trap_Trace(results, start, mins, maxs, end, passEntityNum, contentmask);
	}

	// Now reset all entity contents that have been saved
	for (i = 0; i < j; i++)
	{
		g_entities[dimension_save[i].entityNum].r.contents = dimension_save[i].contents;
	}
}
