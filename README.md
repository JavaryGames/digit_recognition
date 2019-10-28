# Digit Recognition module for Godot 3.x

Usage:

```gdscript
DigitRecognition.recognize(image.get_data())
```

Where `image` is of type Image with a monochromatic format. The input
to  recognize is essentially an array of pixels from a 28x28 image.

This uses the [Kann](https://github.com/attractivechaos/kann) library
to drive the neural network for the digit recognition. The
[MNIST database](http://yann.lecun.com/exdb/mnist/) was used for training
data.

Check the MLP example at [Kann repository](https://github.com/attractivechaos/kann/tree/faa946edec49d93b4c7d4972598471297df419b8/examples#multi-layer-perceptron)
to see how to train the network. The generated file should be put in this
module as `data/mnist_mlp.knw`.

## Compiling

As usual with Godot modules: put the contents of this repo in the `module/digit_recognition`
folder of the Godot source, then build Godot as usual.
