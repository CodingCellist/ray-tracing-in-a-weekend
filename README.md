# Ray Tracing in a Weekend

My implementations of the ray tracing books.

Ray tracing is something I've been highly interested in for a while (along with
photorealistic graphics in general), but my interest really got refuelled when I
discovered the Minecraft SEUS shaders and when Nvidia released their 2nd
generation of RTX cards.

I just like things that produce visual output/feedback. If it looks pretty,
that's even better! ^^

## Book 1: Ray Tracing in One Weekend

This was great fun! Took a lot longer to render things than I thought (the final
image took ~8 **hours** to render on my ThinkPad X1E1 with a Core i7-8750H CPU),
but I also learned a lot along the way. And the render _is_ pretty:

![Many coloured spheres, some made of metal which reflect the scene, some made of glass which refract the horizon and the scene. All are placed on a grey ground. There are 3 big spheres in the middle, one of each material, which really showcase the effects modelled](rtiaw-src/final.png)

Turns out, when you're doing this as a side-project to your PhD, it takes
slightly longer than one weekend... I want to do the next book
("[Ray Tracing: The Next Week][2]"), but given how long the first book took, I
might wait a bit.

## Additional optimisations/modifications

None yet. But I want to give parallelisation a go. We'll see.

# References

[_Ray Tracing in One Weekend_][1]
[_Ray Tracing: The Next Week_][2]

[1]: https://raytracing.github.io/books/RayTracingInOneWeekend.html
[2]: https://raytracing.github.io/books/RayTracingTheNextWeek.html 

# LICENSE

The code in this repository is licensed under the MIT license (see LICENSE).

