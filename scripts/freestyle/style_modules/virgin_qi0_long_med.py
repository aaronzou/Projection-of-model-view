###############################################################################
#                                                                             #
# This file is part of a style sheet example from the Freestyle application   #
# Copyright (C) 2001-2005  Stephane Grabli                                    #
#                                                                             #
# http://freestyle.sourceforge.net                                            #
#                                                                             #
###############################################################################
from Freestyle import *
from logical_operators import *
from PredicatesB1D import *
from shaders import *
from ChainingIterators import *
Operators.select(QuantitativeInvisibilityUP1D(0))
Operators.bidirectionalChain(ChainSilhouetteIterator(), NotUP1D(QuantitativeInvisibilityUP1D(0)))
Operators.select(NotUP1D(pyNatureUP1D(SUGGESTIVE_CONTOUR)))
Operators.select(AndUP1D(pyHigherLengthUP1D(9), NotUP1D(pyHigherLengthUP1D(20))))
Operators.select(pyHighViewMapDensityUP1D(0.07, 5))
shaders_list = 	[
		SamplingShader(2),
		TextureAssignerShader(5),
		SmoothingShader(100, 0.1, 0, 0.2, 0, 0, 0, 1),
		IncreasingThicknessShader(1, 4),
		IncreasingColorShader(0.5,0.0,0.1, 0.6, 0.4, 0.0, 0.0, 0.8),
		pyBackboneStretcherShader(4)
		]
Operators.create(TrueUP1D(), shaders_list)
