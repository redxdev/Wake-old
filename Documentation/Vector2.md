# Vector2 Class Documentation

Vector2 is a userdata type backed by a wrapper around glm::vec2.

Vector2 functions tend to favor creating a new vector over changing the existing
one. The exception to this is the family of `set` functions.

All functions defined inside Vector2 objects (aside from metamethods) may also
be called with the dot syntax on the global `Vector2` table. For example, the
following two lines print the same thing, given `a` and `b`:

    print(a:dot(b))
    print(Vector2.dot(a, b))

Additionally, you may pass a table with the correct number of components in
place of a Vector2 object in most cases:

    print(Vector2.dot({1, 2}, {3, 4}))
    print(Vector2.dot(Vector2.new(1, 2), Vector2.new(3, 4)))

This will work with mathematical operators as well, as long as at least one
operand is a Vector2 (excluding equality).

    local a = Vector2.new(1, 2)
    print(a * {3, 4})
    local b = Vector2.new(3, 4)
    print(a * b) -- same as above

    print({1, 2} * {3, 4}) -- THIS WILL NOT WORK, ONE SIDE NEEDS TO BE A VECTOR2

## Functions

### Vector2.new() : Vector2

Constructor. Equivalent to `Vector2.new(0, 0)`.

### Vector2.new(val : Vector2) : Vector2

Copy constructor. Copies the Vector2 object into a new one.

### Vector2.new(x : number, y : number) : Vector2

Constructor. Creates a new Vector2 with components `x` and `y`.

### Vector2:table() : table

Get the table representation of this Vector3.

### Vector2:get(index : number) : number

Get the component at `index`.

### Vector2:set(index : number, val : number)

Set the component at `index` to `val`.

### Vector2:setAll(x : number, y : number)

Set the values of all components.

### Vector2:dot(val : Vector2) : number

Get the result of the dot product between two vectors.

### Vector2:distance(val : Vector2) : number

Get the distance between two vectors.

### Vector2:length() : number

Get the length of a vector.

### Vector2:apply(f : function) : Vector2

Applies a function `f` to all components in a vector, returning a new vector.

### Vector2:normalized() : Vector2

Get the normalized vector.

### Vector2:reflect(normal : Vector3) : Vector2

Reflect a vector.

### Vector2:refract(normal : Vector3, index : number) : Vector2

Refract a vector.

### String

Vector2 will return itself as a string in the form `(x, y)`.

### Equality

A Vector2 is equal to all other Vector2s with the same components. Due to the
way lua handles tables, this will not work unless both components are Vector2s.

### Length Operator

The length operator of a Vector2 is the number of components it contains (2).
Use the `Vector2:length` function if you want the mathematical length.

### Unary Minus

The negation of a Vector2. Returns a new Vector2 with each component negated.

### Addition

The addition of a Vector2 and another Vector2 is the addition of the individual
components:

    Vector2.new(1, 2) + Vector2.new(3, 4) == Vector2.new(4, 6) -- true

The addition of a Vector2 and a number adds the number to all components of the
Vector2.

### Subtraction

Subtraction works the same way as addition.

### Multiplication

The multiplication of a Vector2 and another Vector2 is the multiplication of the
individual components.

The multiplication of a Vector2 and a number is each component multiplied by the
number.

### Division

Division works the same way as multiplication.

### Comparison Operators

Comparison operators are not defined for Vector2.
