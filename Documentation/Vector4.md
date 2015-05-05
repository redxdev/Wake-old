# Vector4 Class Documentation

Vector4 is a userdata type backed by a wrapper around glm::vec4.

Vector4 functions tend to favor creating a new vector over changing the existing
one. The exception to this is the family of `set` functions.

All functions defined inside Vector4 objects (aside from metamethods) may also
be called with the dot syntax on the global `Vector4` table. For example, the
following two lines print the same thing, given `a` and `b`:

    print(a:dot(b))
    print(Vector4.dot(a, b))

Additionally, you may pass a table with the correct number of components in
place of a Vector4 object in most cases:

    print(Vector4.dot({1, 2, 3, 4}, {5, 6, 7, 8}))
    print(Vector4.dot(Vector4.new(1, 2, 3, 4), Vector4.new(5, 6, 7, 8)))

This will work with mathematical operators as well, as long as at least one
operand is a Vector4 (excluding equality).

    local a = Vector4.new(1, 2, 3, 4)
    print(a * {3, 4, 5, 6})
    local b = Vector2.new(3, 4, 5, 6)
    print(a * b) -- same as above

    print({1, 2, 3, 4} * {3, 4, 5, 6}) -- THIS WILL NOT WORK, ONE SIDE NEEDS TO BE A VECTOR4

## Functions

### Vector4.new() : Vector4

Constructor. Equivalent to `Vector4.new(0, 0, 0, 0)`.

### Vector4.new(val : Vector4) : Vector4

Copy constructor. Copies the Vector4 object into a new one. This function can
take a table instead and copy that into a new Vector4.

### Vector4.new(x : number, y : number, z : number, w : number) : Vector4

Constructor. Creates a new Vector3 with components `x`, `y`, `z`, and `w`.

### Vector4:table() : table

Get the table representation of this Vector4.

### Vector4:get(index : number) : number

Get the component at `index`.

### Vector4:set(index : number, val : number)

Set the component at `index` to `val`.

### Vector4:setAll(x : number, y : number, z : number, w : number)

Set the values of all components.

### Vector4:dot(val : Vector4) : number

Get the result of the dot product between two vectors.

### Vector4:distance(val : Vector4) : number

Get the distance between two vectors.

### Vector4:length() : number

Get the length of a vector.

### Vector4:apply(f : function) : Vector3

Applies a function `f` to all components in a vector, returning a new vector.

### Vector4:normalized() : Vector3

Get the normalized vector.

### Vector4:reflect(normal : Vector4) : Vector4

Reflect a vector.

### Vector4:refract(normal : Vector4, index : number) : Vector4

Refract a vector.

### String

Vector4 will return itself as a string in the form `(x, y, z, w)`.

### Equality

A Vector4 is equal to all other Vector3s with the same components. Due to the
way lua handles tables, this will not work unless both components are Vector4s.

### Length Operator

The length operator of a Vector4 is the number of components it contains (4).
Use the `Vector4:length` function if you want the mathematical length.

### Unary Minus

The negation of a Vector4. Returns a new Vector4 with each component negated.

### Addition

The addition of a Vector4 and another Vector4 is the addition of the individual
components:

    Vector4.new(1, 2, 3, 4) + Vector4.new(4, 5, 6, 7) == Vector4.new(5, 7, 9, 11) -- true

The addition of a Vector4 and a number adds the number to all components of the
Vector4.

### Subtraction

Subtraction works the same way as addition.

### Multiplication

The multiplication of a Vector4 and another Vector4 is the multiplication of the
individual components.

The multiplication of a Vector4 and a number is each component multiplied by the
number.

### Division

Division works the same way as multiplication.

### Comparison Operators

Comparison operators are not defined for Vector4.
