# Tutorial

> This file contains the most important commands for navigating [Helpy](src/Helpy.h).

## Network manipulation

The following commands are used to modify the railway network.

|          <span style="color:yellow">Command</span>          | <span style="color:yellow">Description</span>                                                    |
|:-----------------------------------------------------------:|:-------------------------------------------------------------------------------------------------|
|                 **Change Railway Sources**                  | Changes the <ins>sources</ins> (the stations from which trains are sent) of the railway network. |
|                 **Change Railway Sources**                  | Changes the <ins>sinks</ins> (the stations where trains are sent) of the railway network.        |

## Flow

The following commands display the results of the flow algorithms applied to the network.

|            <span style="color:yellow">Command</span>             | <span style="color:yellow">Description</span>                                                                                                                |
|:----------------------------------------------------------------:|:-------------------------------------------------------------------------------------------------------------------------------------------------------------|
|                   **Calculate Maximum Trains**                   | Calculates the maximum amount of trains that can either:<br/>1. Simultaneously travel between two stations;<br/>2. Simultaneously arrive at a station.       |
|                    **Display Busiest Pairs**                     | Computes the <ins>pairs of stations</ins> that have whose amount of trains that can simultaneously travel between them is the HIGHEST of the entire network. |

## Network analysis

### Resources

The following commands statistically analyse the **train usage** of railway network.<br>
It is important to note that, unlike the commands in the previous section, they take into account the **ENTIRE** railway network.

| <span style="color:yellow">Command</span> | <span style="color:yellow">Description</span>                                                                          |
|:-----------------------------------------:|:-----------------------------------------------------------------------------------------------------------------------|
|       **Display Busiest Stations**        | Computes the top-k <ins>stations</ins> that have the most trains circulating in the railways that are connected to it. |
|       **Display Busiest Districts**       | Computes the top-k <ins>districts</ins> that have the most trains circulating in them.                                 |
|    **Display Busiest Municipalities**     | Computes the top-k <ins>municipalities</ins> that have the most trains circulating in them.                            |

### Expenses

The following command concerns the **monetary cost** of maintaining the railway network.<br>

| <span style="color:yellow">Command</span> | <span style="color:yellow">Description</span>                                                                                                                                                                                                                                                                    |
|:-----------------------------------------:|:-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
|       **Calculate Maximum Trains**        | Calculates the maximum amount of trains that can simultaneously travel between two stations with <ins>minimum cost</ins> for the company.<br/><br/>**NOTE:** This command is the same as the one mentioned [above](#Flow). However, to access this particular functionality, it is necessary to select option 1. |

## Network reduction

The following commands have to do with networks of **reduced connectivity** i.e. networks with at least one railway that cannot be used.

| <span style="color:yellow">Command</span> | <span style="color:yellow">Description</span>                                                                                                                                   |
|:-----------------------------------------:|:--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
|        **Change Railway Network**         | Creates a railway network of reduced connectivity.                                                                                                                              |
|        **Determine Most Affected**        | Computes the top-k stations that were the most affected by the network reduction.<br/><br/>**NOTE:** This command only works if the current network is of reduced connectivity. |
