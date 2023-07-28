# Introduction

Custom rendering is a feature in Repentogon that gives you an extremely fine grained control over 
what gets rendered by the game in any given frame. In particular, it allows you to run custom 
shaders when the game renders individual objects, instead of being limited to running a shader on 
the entire scene once it has been computed.

# On rendering

This section presents how the render system of the game works. It is not crucial to understand it, 
but it can give some insight in case you get stuck on how to achieve something with the tools at your
disposal in Repentogon.

On the PC version of Repentance, the game uses OpenGL for rendering. I will assume that you have a 
passing knowledge of OpenGL in this tutorial. If you wish to get familiar with it, 
[this website](https://learnopengl.com/Introduction) has a very easy to follow tutorial on the 
topic.

Render operations on the C++ side are much more complicated than what the Lua side might suggest. For 
instance, to draw a sprite playing a certain animation at a certain frame, one would simply create the 
sprite, bind an ANM2 to it, configure the animation and frame, and call Render. On the C++ side, this 
results in multiple render operations. More precisely, there will be one render operation for every 
visible animation layer at the given frame. So an ANM2 with an animation that uses two layers will 
result in two render operations on the C++ side.

Render operations are not performed on the fly; they are *queued*. The game uses an array that stores 
the description of every render operation (what is the source texture, where to render it etc.), and 
the `KAGE::Graphics::Manager::apply_frame_images` function iterates over this array and apply the render
operations in the order in which they were queued. 

Render operations are extremely inconsistent. For instance, in order to render six gapers in a room, the 
game will queue six different operations. On the other hand, to render ten bullets fired by a single enemy,
the game will queue a single render operation. The data at your disposal in Lua therefore needs to be 
interpreted relative to the context that is associated with it. See the table **TODO** at **TODO** for 
a breakdown.

The game uses the [`glDrawElements`](https://registry.khronos.org/OpenGL-Refpages/gl4/html/glDrawElements.xhtml) 
function to draw a set of vertices. The `mode` parameter is always set to `GL_TRIANGLES`. Isaac is a sprite-based
game, and as such the game draws rectangles, *i.e.* two triangles.

???- info "C++ devs note"
    Most developers would perform a call to `glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ...)` prior to calling
    `glDrawElements` in order to tell the function where to find the indices of the vertices in the vertex
    buffer. In Isaac no such call is made, and the elements array is always given as the `data` parameter
    of the call to `glDrawElements`. 
    
    Additionnally, the game does not use a vertex array and computes both the vertex array and the elements
    array on the fly when render operations are performed. As a result, you won't find call to `glBindVertexArray`
    in the source of the game.
    
Calls to `glDrawElements` abound across rendering, with the game performing dozens of them in order to render a 
single frame. This is due to the fact that render operations are dequeued and performed one after the other.

The `MC_PRE_OPENGL_RENDER` callback is fired prior to every call to `glDrawElements`.

???- info "C++ devs note"
    To be extremely precise, the execution of the callback happens immediately before the call to `glDrawElements`:
    the C++ stack already holds all the parameters of the function.
    
# Custom rendering

In order to perform custom rendering, you need to attach a function to `MC_PRE_OPENGL_RENDER` callback. This 
function is called with the following parameters :

* The vertex buffer that contains the vertices that are going to be sent to the vertex shader. This is an instance
of the `NativeVertexBuffer` class.
* The elements array indicating in which order the vertices will be drawn. This is an instance of the `ElementsArray`
class.
* The ID of the shader that will be executed. This is a value from the `Renderer.Shaders` enumeration.
* **TODO** The context of the render operation. In other words, what is being drawn.

There are two ways to work in this callback. The first is limited modifications to the input. The second is completely
custom rendering.

## Modifications of the input of `MC_PRE_OPENGL_RENDER`

To indicate to the Repentogon runtime that you want to perform this kind of rendering, return a value from the 
`Renderer.Shaders` enumeration, or `nil`.

Modification here is limited to editing the content of the vertices in the vertex buffer, the indices in the elements
array and the currently bound shader. It is not possible to add or remove vertices, nor is it possible to add or 
remove elements. Furthermore, if the bound shader is changed, then the new shader must be compatible with the previous 
one, *i.e* they must have exactly the same structure.

In order to modify the content of the vertex buffer, you can use the `GetVertex` method of the `NativeVertexBuffer` class.
It will return a representation of the *i*-th vertex (indexed from 0). This representation can be manipulated like a Lua 
table, where the keys are the attributes of an input vertex in the currently bound vertex shader. For instance, if the 
vertex shader has the following structure:

```glsl
attribute vec3 Position;
attribute vec4 Color;
```

Then you have access to the `Position` and `Color` keys on the `Vertex` object returned by `GetVertex`. You can then edit 
these values as you see fit. Example:

```lua
mod:AddCallback(ModCallbacks.MC_PRE_OPENGL_RENDER, function(_, buffer, elements, shader, ctx)
    local vertex = buffer:GetVertex(0)
    vertex.Position.x = vertex.Position.x + 1
end)
```

In order to modify the content of the elements array, you can use the `[]` operator on the object, and give the index 
you want to access. You may use the `#` operator to get the maximum *valid* index you can use.

In order to change the currently bound shader, return either:

* A value of the `Renderer.Shaders` enumeration.
* An object of type `Shader` obtained by a call to `Renderer.Shader()`

???- info "Rationale of the design"
    Due to the way the game manages the vertex buffer and the elements array on his side, adding / removing vertices at 
    will would require copying the entire content of these buffers into new ones. As such, I (Sylmir)
    decided to consciously limit what can be done with this rendering mode.
    
## Completely custom rendering

In order to use completely custom rendering, one must return a `Pipeline` object in the `MC_PRE_OPENGL_RENDER`
callback.

In order to create a `Pipeline`, use the `Renderer.Pipeline()` function.

A `Pipeline` object acts as a container for a sequence of render passes on a *single* input. 
In other words, the purpose of the `Pipeline` is to allow you to alter an existing render operation.
If you want to render an arbitrary sprite, then first create this sprite, use Render() on it, and 
then change its rendering in this callback. Do not use the callback as a way to render anything.

To understand the idea of "sequence of render passes", consider the following code: 

```lua

mod.EdgeDetectionShader = <create shader>
mod.BloomShader = <create shader>

mod:AddCallback(ModCallbacks.MC_PRE_OPENGL_RENDER, function(_, vertexBuffer, elements, shader, ctx)
    local pipeline = Renderer.Pipeline()
    local edgeBuffer = pipeline:NewPass(#vertexBuffer, elements, mod.EdgeDetectionShader)
    <copy content of vertexBuffer into edgeBuffer>
    local bloomBuffer = pipeline:NewPass(4, <define elements array>, mod.BloomShader)
    <bind the uniforms in the two shaders>
    return pipeline
end)
```

In the function attached to the callback, we create new `Pipeline` and register two render passes in 
it, using the `NewPass` method (read more). The first pass will use an edge detection shader. The
second pass will work on the output of the first pass and further apply a bloom shader on this result.
We have a pipeline consisting of two sequential passes.

The `NewPass` function creates a new pass in a pipeline. You need three elements to define a pass: 
how many vertices will be **defined**, the elements array, and the shader program that will be used 
to perform the rendering. The function then returns a `VertexBuffer` object containing the specified
number of vertices.

### Creating a shader

In order to create a shader, you need to use the `Renderer.Shader` function. This function takes three
parameters: the path to the vertex shader file, the path to the fragment shader file, and a descriptor
(`VertexDescriptor`).

To create a `VertexDescriptor`, use the `Renderer.VertexDescriptor()` function.

A `VertexDescriptor` acts as a representation of the vertex shader's attributes. For example, consider 
the following vertex shader.

```glsl
attribute vec3 Position;
attribute vec4 Color;
varying vec4 ColorOut;

uniform mat4 Transform;

void main() {
    ColorOut = Color;
    gl_Position = Transform * vec4(Position, 1);
}
```

This vertex shader has two attributes, `Position` of type `vec3` and `Color` of type `vec4`. Uniforms do 
not count as attributes.

???- info "Modern OpenGL"
    If you write your shaders in a reasonnably recent version of the GLSL language, the `attribute` keyword is
    either deprecated or outright removed, and you use the `in` keyword instead.
    
A `VertexDescriptor` for this vertex shader is a Lua representation of these attributes. The purpose of
this descriptor is to let Lua structure the `Vertex` objects in a `VertexBuffer` in a way that mirrors 
the attributes of the vertex shader.

To define the attributes of a `VertexDescriptor` use the `AddAttribute` method. This method takes as 
parameters the name of the attribute (a string) as well as the type of the attribute (a value from the
`Renderer.GLSLType` enumeration). To keep with the previous example, here is how one would define the 
descriptor of the previous vertex shader, and create a shader object:

```lua
local descriptor = Renderer.VertexDescriptor()
descriptor:AddAttribute("Position", Renderer.GLSLType.VEC3)
descriptor:AddAttribute("Color", Renderer.GLSLType.VEC4)
local shader = Renderer.Shader("shaders/vertex.vs", "shaders/fragment.fs", descriptor)
```

???- warn "Copies and references"
    When a `VertexDescriptor` is used to create a shader, its content are copied inside the  C++
    representation of the shader, and not stored by reference. As a consequence, further 
    modifications of the descriptor will not be reflected in the shader. Furthermore, this 
    avoids an issue where losing all references to the descriptor on the Lua side could 
    accidentally cause a memory corruption on the C++ side.
    
???- info "Rationale of the copy"
    The description of a shader is not supposed to change as the program runs. Therefore I think 
    it makes no sense to store a reference to the descriptor. Furthermore, this avoids confusion when
    accidental modifications to the descriptor result in black squares being rendered.
    
### Creating an elements array

In order to create an elements array, use the `Renderer.NewElements()` functions. This function takes 
as parameters the number of vertices that will be rendered.

You can then use the `[]` operator with a number as parameter to access the element at a given index.
Using a number outside the range defined in `NewElements` results in an error.

The code below shows an example on how to define an elements array that can be used to render a rectangle.

```lua
local elements = Renderer.NewElements(6)
elements[0] = 0
elements[1] = 1
elements[2] = 2
elements[3] = 2
elements[4] = 1
elements[5] = 3
```

### Defining the vertices

Before the call to `Pipeline:NewPass()` returns, the runtime ensures that the values defined in the 
elements array are consistent with the number of requested vertices in the vertex buffer.

The call returns a `VertexBuffer` object, on which you can use the `GetVertex()` method to get access
to the *i*-th vertex (0-based) as a `Vertex`. This `Vertex` contains attributes named and typed after
the content of the `VertexDescriptor` object. Keeping with our above example: 

```lua
local buffer = pipeline:NewPass(...)
local vertex = buffer:GetVertex(0)
vertex.Position = Renderer.Vec3(1, 0, 0)
vertex.Color = Renderer.Vec4(0.2, 0.3, 0.4, 0.5)
```

### Binding uniforms 

The final step in custom rendering is to bind values to the uniforms of shaders.

In order to bind a uniform in a shader, use the appropriate `Bind*` function on a 
`Shader` object. All these functions take as parameter the name of the uniform to
bind and the value to bind to it.

Due to the way OpenGL works, every time a value is bound to a uniform further 
modifications of the variable holding this value have no effect on the uniform's 
value.

The uniform you'll probably need in almost any situation is the projection matrix, 
that converts coordinates from view space (the coordinates returned by the 
`WorldToScreen()` function) into clip space (the screen on which rendering is being
done). This matrix can be accessed by using the `Renderer.GetProjectionMatrix()`
function.

```lua
mod.EdgeDetectionShader:BindMat4("Transform", Renderer.GetProjectionMatrix())
```

### Successive passes and the view space

One of the difficulties in writing multiple successive passes is to consider what the
vertices of the all passes from the second onwards are. In particular, what are the 
coordinates (and in which space they are) of the vertices. 

In the vanilla API, if  multiple mods define custom shaders, they are executed in a sequence. 
Each time, the input vertices are positionned at the boundaries of the view space, in such a way that
transforming them through the projection matrix results in the boundaries of clip space.

In the custom render system, you are free to use whatever method you feel is the most suited to 
what you want to achieve.

And there you go, you're free to do whatever you want with rendering now. Have fun.