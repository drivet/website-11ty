---
title: The Graphics Pipeline
---

## More fun than it sounds...

I wanted to do something constructive with my PEY term, so I decided to
learn what I could about 3D graphics.  When I say "learn" I don't mean any
particular library or product like OpenGL or DirectX or POV-Ray (although I
did have some fun with POV-Ray).  I decided to learn about the math involved
so I could write my own 3D library.  I was taking home a paycheck and I was
feeling brave.

So here we are. The library's name is GP, for Graphics Pipeline (I never
said I was creative). It is low level polygon renderer with a design similar
to OpenGL, although not identical (OpenGL, for instance does not allow
oblique projections, while this API does). I think the shading might be a
bit off but...whatever. It was all in good fun and maybe I'll fix it
someday.

## Gallery

### Sine wave carpet
<table style="border: none">
<tr>
<td style="border: none"><a href="/oldsite/gp_img/carpet10.jpg"><img src="/oldsite/gp_img/carpet10_thumb.jpg"/></a></td>
<td style="border: none"><a href="/oldsite/gp_img/carpet20.jpg"><img src="/oldsite/gp_img/carpet20_thumb.jpg"/></a></td>
<td style="border: none"><a href="/oldsite/gp_img/carpet40.jpg"><img src="/oldsite/gp_img/carpet40_thumb.jpg"/></a></td>
</tr>
<tr>
<td style="border: none">200 triangles</td>
<td style="border: none">800 triangle</td>
<td style="border: none">3200 triangles</td>
</tr>
</table>

### Simple water wave
<table style="border: none">
<tr>
<td style="border: none"><a href="/oldsite/gp_img/yelcos.jpg"><img src="/oldsite/gp_img/yelcos_thumb.jpg"/></a></td>
<td style="border: none"><a href="/oldsite/gp_img/redcos.jpg"><img src="/oldsite/gp_img/redcos_thumb.jpg"/></a></td>
<td style="border: none"><a href="/oldsite/gp_img/bgcos.jpg"><img src="/oldsite/gp_img/bgcos_thumb.jpg"/></a></td>
</tr>
<tr>
<td style="border: none">Yellow</td>
<td style="border: none">Red</td>
<td style="border: none">Blue solid and green wire</td>
</tr>
</table>

### Mobius strip

<table style="border: none">
<tr>
<td style="border: none"><a href="/oldsite/gp_img/bluemobius.jpg"><img src="/oldsite/gp_img/bluemobius_thumb.jpg"/></a></td>
<td style="border: none"><a href="/oldsite/gp_img/yellowmobius.jpg"><img src="/oldsite/gp_img/yellowmobius_thumb.jpg"/></a></td>
<td style="border: none"><a href="/oldsite/gp_img/colormobius.jpg"><img src="/oldsite/gp_img/colormobius_thumb.jpg"/></a></td>
</tr>
<tr>
<td style="border: none">Blue</td>
<td style="border: none">Yellow</td>
<td style="border: none">Psychedelic</td>
</tr>
</table>

### Source Code
* [gpbase.h](/oldsite/gp_src/gpbase.h)
* [gpbase.c](/oldsite/gp_src/gpbase.c)
* [algebra.h](/oldsite/gp_src/algebra.h)
* [algebra.c](/oldsite/gp_src/algebra.c)
* [clip.h](/oldsite/gp_src/clip.h)
* [clip.c](/oldsite/gp_src/clip.c)
* [light.h](/oldsite/gp_src/light.h)
* [light.c](/oldsite/gp_src/light.c)
* [pipe.h](/oldsite/gp_src/pipe.h)
* [pipe.c](/oldsite/gp_src/pipe.c)
* [raster.h](/oldsite/gp_src/raster.h)
* [raster.c](/oldsite/gp_src/raster.c)
* [view.h](/oldsite/gp_src/view.h)
* [view.c](/oldsite/gp_src/view.c)
