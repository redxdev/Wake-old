# Vector3 Class Documentation

Vector3 is a userdata type backed by a wrapper around glm::vec3.

Vector3 functions tend to favor creating a new vector over changing the existing
one. The exception to this is the family of `set` functions.

All functions defined inside Vector3 objects (aside from metamethods) may also
be called with the dot syntax on the global `Vector3` table. For example, the
following two lines print the same thing, given `a` and `b`:

    print(a:dot(b))
    print(Vector3.dot(a, b))

Additionally, you may pass a table with the correct number of components in
place of a Vector3 object in most cases:

    print(Vector3.dot({1, 2, 3}, {4, 5, 6}))
    print(Vector3.dot(Vector4.new(1, 2, 3), Vector4.new(4, 5, 6)))

This will work with mathematical operators as well, as long as at least one
operand is a Vector3 (excluding equality).

    local a = Vector4.new(1, 2, 3)
    print(a * {3, 4, 5})
    local b = Vector4.new(3, 4, 5)
    print(a * b) -- same as above

    print({1, 2, 3} * {3, 4, 5}) -- THIS WILL NOT WORK, ONE SIDE NEEDS TO BE A VECTOR3

## Functions

### Vector3.new() : Vector3

Constructor. Equivalent to `Vector3.new(0, 0, 0)`.

### Vector3.new(val : Vector3) : Vector3

Copy constructor. Copies the Vector3 object into a new one. This function can
take a table instead and copy that into a new Vector3.

### Vector3.new(x : number, y : number, z : number) : Vector3

Constructor. Creates a new Vector3 with components `x`, `y`, and `z`.

### Vector3:table() : table

Get the table representation of this Vector3.

### Vector3:get(index : number) : number

Get the component at `index`.

### Vector3:set(index : number, val : number)

Set the component at `index` to `val`.

### Vector3:setAll(x : number, y : number, z : number)

Set the values of all components.

### Vector3:dot(val : Vector3) : number

Get the result of the dot product between two vectors.

### Vector3:distance(val : Vector3) : number

Get the distance between two vectors.

### Vector3:length() : number

Get the length of a vector.

### Vector3:apply(f : function) : Vector3

Applies a function `f` to all components in a vector, returning a new vector.

### Vector3:normalized() : Vector3

Get the normalized vector.

### Vector3:reflect(normal : Vector3) : Vector3

Reflect a vector.

### Vector3:refract(normal : Vector3, index : number) : Vector3

Refract a vector.

### Vector3:cross(val : Vector3) : Vector3

Get the result of the cross product between two vectors.

### String

Vector3 will return itself as a string in the form `(x, y, z)`.

### Equality

A Vector3 is equal to all other Vector3s with the same components. Due to the
way lua handles tables, this will not work unless both components are Vector3s.

### Length Operator

The length operator of a Vector3 is the number of components it contains (3).
Use the `Vector3:length` function if you want the mathematical length.

### Unary Minus

The negation of a Vector3. Returns a new Vector3 with each component negated.

### Addition

The addition of a Vector3 and another Vector3 is the addition of the individual
components:

    Vector3.new(1, 2, 3) + Vector3.new(4, 5, 6) == Vector3.new(5, 7, 9) -- true

The addition of a Vector3 and a number adds the number to all components of the
Vector3.

### Subtraction

Subtraction works the same way as addition.

### Multiplication

The multiplication of a Vector3 and another Vector3 is the multiplication of the
individual components.

The multiplication of a Vector3 and a number is each component multiplied by the
number.

### Division

Division works the same way as multiplication.

### Comparison Operators

Comparison operators are not defined for Vector3.
