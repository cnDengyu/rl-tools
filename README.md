<div align="center">
  <center><h1><span style="color:#7DB9B6">RLtools</span>: A Fast, Portable Deep Reinforcement Learning Library for Continuous Control</h1></center>
</div>


<div align="center">
<img src="https://github.com/rl-tools/media/blob/master/overview.png"/ width=500>  
</div>

<p align="center">
  <a href="https://arxiv.org/abs/2306.03530">Paper on arXiv</a> | <a href="https://rl.tools">Live demo (browser)</a> | <a href="https://discord.gg/kbvxCavb5h">Discord</a>
  </br>
</br>
  <a href="https://github.com/rl-tools/rl-tools/actions/workflows/tests-backend.yml">
  <img src="https://github.com/rl-tools/rl-tools/actions/workflows/tests-backend.yml/badge.svg" alt="Documentation">
  </a>
  <a href="https://codecov.io/gh/rl-tools/rl-tools" >
  <img src="https://codecov.io/gh/rl-tools/rl-tools/graph/badge.svg?token=3TJZ635O8V"/>
  </a>
  <a href="https://docs.rl.tools">
  <img src="https://img.shields.io/badge/Documentation-Read%20the%20Docs-blue.svg" alt="Documentation">
  </a>
</br>
  <a href="https://mybinder.org/v2/gh/rl-tools/documentation/binder?labpath=01-Containers.ipynb">
  <img src="https://mybinder.org/badge_logo.svg" alt="Run tutorials on Binder">
  </a>
  <a href="https://colab.research.google.com/github/rl-tools/documentation/blob/master/docs/09-Python%20Interface.ipynb">
  <img src="https://colab.research.google.com/assets/colab-badge.svg" alt="Run Example on Colab">
  </a>
</p>



<div align="center">
<img src="https://github.com/rl-tools/media/blob/master/pendulum_v1_inference.gif" alt="animated" height='200'/>
<img src="https://github.com/rl-tools/media/blob/master/racing_car.gif" alt="animated" height='200'/>
</div>
<div align="center">
    Trained on a 2020 MacBook Pro (M1) using <span style="color:#7DB9B6">RLtools</span> TD3
</div>
</br>

<div align="center">
<img src="https://github.com/rl-tools/media/blob/master/rl_tools_mujoco_ant_ppo.gif" alt="animated" width='350'/>  
</div>

<div align="center">
    Trained on a 2020 MacBook Pro (M1) using <span style="color:#7DB9B6">RLtools</span> PPO
</div>
</br>


## Benchmarks

<div align="center">
<img src="https://github.com/rl-tools/media/blob/master/benchmark_horizontal_ppo.png"/ width=300>  
<img src="https://github.com/rl-tools/media/blob/master/benchmark_horizontal_sac.png"/ width=300>
</div>
<div align="center">
    Benchmarks of training the Pendulum swing-up using different RL libraries (PPO and SAC respectively)
</div>
</br>
<div align="center">
<img src="https://github.com/rl-tools/media/blob/master/benchmark_vertical.png"/ width=350>  
</div>
<div align="center">
    Benchmarks of training the Pendulum swing-up on different devices (SAC, RLtools)
</div>

</br>
<div align="center">
<img src="https://github.com/rl-tools/media/blob/master/microcontroller_inference.png"/ width=600>  
</div>
<div align="center">
    Benchmarks of the inference frequency for a two-layer [64, 64] fully-connected neural network across different microcontrollers (types and architectures).
</div>


## Algorithms
| Algorithm | Example                                                                                                                                                                                                                                                                                |
|-----------|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| **TD3**   | [Pendulum](./src/rl/environments/pendulum/td3/cpu/standalone.cpp), [Racing Car](./src/rl/environments/car/car.cpp), [MuJoCo Ant-v4](./src/rl/environments/mujoco/ant/td3/training.h), [Acrobot](./src/rl/environments/acrobot/td3/acrobot.cpp)                                         |
| **PPO**   | [Pendulum](./src/rl/environments/pendulum/ppo/cpu/training.cpp), [Racing Car](./src/rl/environments/car/training_ppo.h), [MuJoCo Ant-v4 (CPU)](./src/rl/environments/mujoco/ant/ppo/cpu/training.h), [MuJoCo Ant-v4 (CUDA)](./src/rl/environments/mujoco/ant/ppo/cuda/training_ppo.cu) |
| **SAC**   | [Pendulum (CPU)](./src/rl/environments/pendulum/sac/cpu/training.cpp), [Pendulum (CUDA)](./src/rl/environments/pendulum/sac/cuda/sac.cu), [Acrobot](./src/rl/environments/acrobot/sac/acrobot.cpp)                                                                                     |

## Projects Based on <span style="color:#7DB9B6">RLtools</span>
- Learning to Fly in Seconds: [GitHub](https://github.com/arplaboratory/learning-to-fly) / [arXiv](https://arxiv.org/abs/2311.13081) / [YouTube](https://youtu.be/NRD43ZA1D-4) / [IEEE Spectrum](https://spectrum.ieee.org/amp/drone-quadrotor-2667196800)


# Getting Started
Simple example on how to implement your own environment and train a policy using PPO:

Clone and checkout:
```
git clone https://github.com/rl-tools/example
cd example
git submodule update --init external/rl_tools
```
build and run:
```
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
./my_pendulum
```

Note this example does not have dependencies and should work on any system with CMake and a C++ 17 compiler.

# Documentation
The documentation is available at [docs.rl.tools](https://docs.rl.tools) and consists of C++ notebooks. You can also run them locally to tinker around:

```
docker run -p 8888:8888 rltools/documentation
```
After running the Docker container, open the link that is displayed in the CLI (http://127.0.0.1:8888/...) in your browser and enjoy tinkering!

| Chapter | Documentation                                                                                 | Interactive Notebook                                                                                                                                                                                                                                                                               |
|---------|-----------------------------------------------------------------------------------------------|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| 0       | [Overview                              ](https://docs.rl.tools/overview.html)                 | -                                                                                                                                                                                                                                                                                                  |
| 1       | [Containers                            ](https://docs.rl.tools/01-Containers.html)            | [![Binder](https://mybinder.org/badge_logo.svg)](https://mybinder.org/v2/gh/rl-tools/documentation/binder?labpath=01-Containers.ipynb)                                                                                                                                                             | 
| 2       | [Multiple Dispatch                     ](https://docs.rl.tools/02-Multiple%20Dispatch.html)   | [![Binder](https://mybinder.org/badge_logo.svg)](https://mybinder.org/v2/gh/rl-tools/documentation/binder?labpath=02-Multiple%20Dispatch.ipynb)                                                                                                                                                    | 
| 3       | [Deep Learning                         ](https://docs.rl.tools/03-Deep%20Learning.html)       | [![Binder](https://mybinder.org/badge_logo.svg)](https://mybinder.org/v2/gh/rl-tools/documentation/binder?labpath=03-Deep%20Learning.ipynb)                                                                                                                                                        | 
| 4       | [CPU Acceleration                      ](https://docs.rl.tools/04-CPU%20Acceleration.html)    | [![Binder](https://mybinder.org/badge_logo.svg)](https://mybinder.org/v2/gh/rl-tools/documentation/binder?labpath=04-CPU%20Acceleration.ipynb)                                                                                                                                                     | 
| 5       | [MNIST Classification                  ](https://docs.rl.tools/05-MNIST%20Classification.html) | [![Binder](https://mybinder.org/badge_logo.svg)](https://mybinder.org/v2/gh/rl-tools/documentation/binder?labpath=05-MNIST%20Classification.ipynb)                                                                                                                                                 | 
| 6       | [Deep Reinforcement Learning           ](https://docs.rl.tools/06-Deep%20Reinforcement%20Lear) | [![Binder](https://mybinder.org/badge_logo.svg)](https://mybinder.org/v2/gh/rl-tools/documentation/binder?labpath=06-Deep%20Reinforcement%20Learning.ipynb)                                                                                                                                        | 
| 7       | [The Loop Interface                    ](https://docs.rl.tools/07-The%20Loop%20Interface.html) | [![Binder](https://mybinder.org/badge_logo.svg)](https://mybinder.org/v2/gh/rl-tools/documentation/binder?labpath=07-The%20Loop%20Interface.ipynb)                                                                                                                                                 | 
| 8       | [Custom Environment                    ](https://docs.rl.tools/08-Custom%20Environment.html)  | [![Binder](https://mybinder.org/badge_logo.svg)](https://mybinder.org/v2/gh/rl-tools/documentation/binder?labpath=08-Custom%20Environment.ipynb)                                                                                                                                                   | 
| 9       | [Python Interface                      ](https://docs.rl.tools/09-Python%20Interface.html)                | [![Run Example on Colab](https://colab.research.google.com/assets/colab-badge.svg)](https://colab.research.google.com/github/rl-tools/documentation/blob/master/docs/09-Python%20Interface.ipynb) | 


[//]: # (## Content)

[//]: # (- [Getting Started]&#40;#getting-started&#41;)

[//]: # (  - [Cloning the Repository]&#40;#cloning-the-repository&#41;)

[//]: # (  - [Docker]&#40;#docker&#41;)

[//]: # (  - [Native]&#40;#native&#41;)

[//]: # (    - [Unix &#40;Linux and macOS&#41;]&#40;#unix-linux-and-macos&#41;)

[//]: # (    - [Windows]&#40;#windows&#41;)

[//]: # (- [Embedded Platforms]&#40;#embedded-platforms&#41;)

[//]: # (- [Naming Convention]&#40;#naming-convention&#41;)

[//]: # (- [Citing]&#40;#citing&#41;)

# Repository Structure
To build the examples from source (either in Docker or natively), first the repository should be cloned.
Instead of cloning all submodules using `git clone --recursive` which takes a lot of space and bandwidth we recommend cloning the main repo containing all the standalone code for <span style="color:#7DB9B6">RLtools</span> and then cloning the required sets of submodules later:
```
git clone https://github.com/rl-tools/rl-tools.git rl_tools
```
#### Cloning submodules
There are three classes of submodules:
1. External dependencies (in `external/`)
   * E.g. HDF5 for checkpointing, Tensorboard for logging, or MuJoCo for the simulation of contact dynamics
2. Examples/Code for embedded platforms (in `embedded_platforms/`)
3. Redistributable dependencies (in `redistributable/`)
4. Test dependencies (in `tests/lib`)
4. Test data (in `tests/data`)

These sets of submodules can be cloned incrementally/independent of each other.
For most use-cases (like e.g. most of the Docker examples) you should clone the submodules for external dependencies:
```
cd rl_tools
```
```
git submodule update --init --recursive -- external
```

The submodules for the embedded platforms, the redistributable binaries and test dependencies/data can be cloned in the same fashion (by replacing `external` with the appropriate folder from the enumeration above). 
Note: Make sure that for the redistributable dependencies and test data `git-lfs` is installed (e.g. `sudo apt install git-lfs` on Ubuntu) and activated (`git lfs install`) otherwise only the metadata of the blobs is downloaded.

### Docker
If you would like to take advantage of the features that require additional dependencies, but don't want to install them on your machine yet, you can use Docker. In our experiments on Linux using the NVIDIA container runtime we were able to achieve close to native performance.
[Docker instructions & examples](examples/docker/README.MD)
While it depends on personal preferences, we believe that there are good reasons (ease of debugging, usage of IDEs etc.) to run everything natively when developing. We make sure that the additional dependencies requried for the full feature set are not invasive and usually available through your systems package manager. We believe `sudo ./setup.sh` is harmful and should not exist. Instead we make the setup explicit so that users maintain agency over their systems. 

### Native
For maximum performance and malleability for research and development we recommend to run <span style="color:#7DB9B6">RLtools</span> natively. Since <span style="color:#7DB9B6">RLtools</span> itself is dependency free the most basic examples don't need any platform setup. However, for an improved experience, we support HDF5 checkpointing and Tensorboard logging as well as optimized BLAS libraries which comes with some system-dependent requirements. 

#### [Unix (Linux and macOS) instructions & examples](examples/unix/README.MD)

#### [Windows instructions & examples](examples/windows/README.MD)

### Python Interface

We provide Python bindings that available as `rltools` through PyPI (the pip package index). Note that using Python Gym environments can slow down the trianing significantly compared to native <span style="color:#7DB9B6">RLtools</span> environments.
```
pip install rltools gymnasium
```
Usage:
```
from rltools import SAC
import gymnasium as gym
from gymnasium.experimental.wrappers import RescaleActionV0

seed = 0xf00d
def env_factory():
    env = gym.make("Pendulum-v1")
    env = RescaleActionV0(env, -1, 1)
    env.reset(seed=seed)
    return env

sac = SAC(env_factory)
state = sac.State(seed)

finished = False
while not finished:
    finished = state.step()
```
You can find more details in the [Python Interface documentation](https://docs.rl.tools/09-Python%20Interface.html) and from the repository [rl-tools/python-interface](https://github.com/rl-tools/python-interface).

## Embedded Platforms
### Inference & Training
- [iOS](https://github.com/rl-tools/ios)
- [teensy](./embedded_platforms)
### Inference
- [Crazyflie](embedded_platforms/crazyflie)
- [ESP32](embedded_platforms)
- [PX4](embedded_platforms)

## Naming Convention
We use `snake_case` for variables/instances, functions as well as namespaces and `PascalCase` for structs/classes. Furthermore, we use upper case `SNAKE_CASE` for compile-time constants. 

## Citing
When using <span style="color:#7DB9B6">RLtools</span> in an academic work please cite our publication using the following Bibtex citation:
```
@misc{eschmann2023rltools,
      title={RLtools: A Fast, Portable Deep Reinforcement Learning Library for Continuous Control}, 
      author={Jonas Eschmann and Dario Albani and Giuseppe Loianno},
      year={2023},
      eprint={2306.03530},
      archivePrefix={arXiv},
      primaryClass={cs.LG}
}
```
