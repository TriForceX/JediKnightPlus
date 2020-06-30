jkmod_hitbox
{
	nopicmip
	{
		map gfx/misc/jkmod_hitbox.tga
		blendFunc GL_ONE GL_ONE
		rgbGen vertex
	}
}
jkmod_hitbox_nocull
{
	nopicmip
	cull none
	{
		map gfx/misc/jkmod_hitbox.tga
		blendFunc GL_ONE GL_ONE
		rgbGen vertex
	}
}
jkmod_chat_icon
{
	nopicmip
	nomipmaps
    {
        map gfx/mp/jkmod_chat_icon
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
    }
}
models/players/hats/cap
{
	q3map_nolightmap
	cull	twosided
	{
		map models/players/hats/cap
		rgbGen lightingDiffuse
	}
}

models/players/hats/cringe
{
	q3map_nolightmap
	cull	twosided
	{
		map models/players/hats/cringe
		rgbGen lightingDiffuse
	}
}

models/players/hats/cringe_prop
{
	q3map_nolightmap
	cull	twosided
	{
		map models/players/hats/cringe_prop
		alphaFunc GE128
		rgbGen vertex
		tcMod rotate 300
	}
}
gfx/2d/net
{
    nopicmip
    nomipmaps
    {
        map gfx/2d/net
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
    }
}
models/map_objects/mp/flag_post
{
	q3map_nolightmap
	q3map_onlyvertexlighting
	sort seeThrough
	cull	disable
	{
		map models/map_objects/mp/flag_post
		blendFunc GL_ONE GL_ZERO
		rgbGen lightingDiffuse
	}
}
models/map_objects/mp/flag
{
	q3map_nolightmap
	q3map_onlyvertexlighting
	sort seeThrough
	cull	disable
    {
        map models/map_objects/mp/flag
        blendFunc GL_ONE GL_ZERO
        rgbGen lightingDiffuse
    }
}

models/map_objects/mp/flag2
{
	q3map_nolightmap
	q3map_onlyvertexlighting
	sort seeThrough
	cull	disable
    {
        map models/map_objects/mp/flag2
        blendFunc GL_ONE GL_ZERO
        rgbGen lightingDiffuse
    }
}
