.. _model-shapes:

Hierarchy of Charge Assignment Schemes
======================================

.. sectionauthor:: Klaus Steiniger

In PIConGPU, the cloud shapes :math:`S^n(x)` are pre-integrated to *assignment functions* :math:`W^n(x)`.

.. math::
   W^n(x) = \Pi(x) \ast S^n(x) = \int\limits_{-\infty}^{+\infty} \Pi(x^\prime) S^n(x^\prime - x) dx^\prime\,, \text{ where }
   \Pi(x) = \left\{\begin{array}{ll}
            0 & |x| \gt \frac{1}{2} \\
            \frac{1}{2} & |x| = \frac{1}{2} \\
            1 & |x| \lt \frac{1}{2}
      \end{array}\right.

is the top-hat function and :math:`\ast` the convolution.

Evaluating the assignment functions at mesh points directly provides the fraction of charge from the marker assigned to that point.

The assignment functions are implemented as B-splines.
The zeroth order assignment function :math:`W^0` is the top-hat function :math:`\Pi`.
It represents charge assignment to the nearest mesh point only, resulting in a stepwise charge density distribution.
Therefore, it should not be used.
The assignment function of order :math:`n` is generated by convolution of the assignment function of order :math:`n-1` with the top-hat function

.. math::
   W^n(x) = W^{n-1}(x) \ast \Pi(x) = \int\limits_{-\infty}^{+\infty} W^{n-1}(x^\prime) \Pi(x^\prime - x) dx^\prime\,.

The three dimensional assignment function is a multiplicative union of B-splines :math:`W^n(x,y,z) = W^n(x) W^n(y) W^n(z)`.

PIConGPU implements these up to order :math:`n=4`.
The naming scheme follows [HockneyEastwood]_, tab. 5-1, p. 144, where the name of a scheme
is defined by the visual form of its cloud shape :math:`S`.


.. table::
    :widths: auto
    :name: assignment_schemes_hierarchy

    +---------------------------------------+-------+----------------------------+
    | Scheme                                | Order | Assignment function        |
    +=======================================+=======+============================+
    | NGP (nearest-grid-point)              | 0     | stepwise                   |
    +---------------------------------------+-------+----------------------------+
    | CIC (cloud-in-cell)                   | 1     | piecewise linear spline    |
    +---------------------------------------+-------+----------------------------+
    | TSC (triangular shaped cloud)         | 2     | piecewise quadratic spline |
    +---------------------------------------+-------+----------------------------+
    | PQS (piecewise quadratic cloud shape) | 3     | piecewise cubic spline     |
    +---------------------------------------+-------+----------------------------+
    | PCS (piecewise cubic cloud shape)     | 4     | piecewise quartic spline   |
    +---------------------------------------+-------+----------------------------+

References
----------

.. [HockneyEastwood]
        R.W. Hockney, J.W. Eastwood.
        *Computer Simulation Using Particles*,
        CRC Press (1988),
        ISBN 0-85274-392-0
