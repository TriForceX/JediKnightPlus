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
models/items/jkmod_hats/cap
{
	q3map_nolightmap
	cull	twosided
	{
		map models/items/jkmod_hats/cap
		rgbGen lightingDiffuse
	}
}

models/items/jkmod_hats/cringe
{
	q3map_nolightmap
	cull	twosided
	{
		map models/items/jkmod_hats/cringe
		rgbGen lightingDiffuse
	}
}

models/items/jkmod_hats/cringe_extra
{
	q3map_nolightmap
	cull	twosided
	{
		map models/items/jkmod_hats/cringe_extra
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
gfx/hud/jkmod_useablehint
{
    {
        map gfx/hud/jkmod_useablehint_circle
        blendFunc GL_ONE GL_ONE
        rgbGen wave sawtooth 0 1 0 0.75
    }
    {
        map gfx/hud/jkmod_useablehint
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
    }
}
gfx/2d/numbers/zero
{
    nopicmip
    nomipmaps
    {
        map gfx/2d/numbers/zero
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen vertex
    }
}
gfx/2d/numbers/one
{
    nopicmip
    nomipmaps
    {
        map gfx/2d/numbers/one
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen vertex
    }
}
gfx/2d/numbers/two
{
    nopicmip
    nomipmaps
    {
        map gfx/2d/numbers/two
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen vertex
    }
}
gfx/2d/numbers/three
{
    nopicmip
    nomipmaps
    {
        map gfx/2d/numbers/three
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen vertex
    }
}
gfx/2d/numbers/four
{
    nopicmip
    nomipmaps
    {
        map gfx/2d/numbers/four
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen vertex
    }
}
gfx/2d/numbers/five
{
    nopicmip
    nomipmaps
    {
        map gfx/2d/numbers/five
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen vertex
    }
}
gfx/2d/numbers/six
{
    nopicmip
    nomipmaps
    {
        map gfx/2d/numbers/six
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen vertex
    }
}
gfx/2d/numbers/seven
{
    nopicmip
    nomipmaps
    {
        map gfx/2d/numbers/seven
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen vertex
    }
}
gfx/2d/numbers/eight
{
    nopicmip
    nomipmaps
    {
        map gfx/2d/numbers/eight
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen vertex
    }
}
gfx/2d/numbers/nine
{
    nopicmip
    nomipmaps
    {
        map gfx/2d/numbers/nine
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen vertex
    }
}
gfx/2d/numbers/minus
{
    nopicmip
    nomipmaps
    {
        map gfx/2d/numbers/minus
        blendFunc GL_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
		rgbGen vertex
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
gfx/2d/jkmod_jsense
{
	nomipmaps
	cull	twosided
    {
        map gfx/2d/jkmod_jedisns
        blendFunc GL_ONE GL_ONE
        glow
        tcMod rotate 5
        tcMod stretch sin 1.4 0.05 1 0.1
    }
    {
        map gfx/2d/jkmod_jedisns1
        blendFunc GL_ONE_MINUS_DST_COLOR GL_ONE
        tcMod stretch sin 1.4 0.07 1 0.1
        tcMod rotate 2
    }
}