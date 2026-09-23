# Modeling_of_Social_Influence

## Question to answer

How does what a person thinks change according to their environment?

## What exactly does it model?

Each person is represented as an **agent** with:

- An opinion (it can be binary: A/B, or continuous: from 0 to 10).
- An environment: family, friends, neighbors, work, media, social networks.
- Update rules: how does my opinion change when interacting with others?

The environment influences through:

- **Exposure:** what I see and hear.
- **Conformity:** pressure to belong to the group.
- **Persuasion:** arguments, leaders, media.
- **Identity:** whether the topic touches my group, religion, class, or region.
- **Algorithms:** what the platform shows me reinforces or expands my view.

## Typical models, from lower to higher complexity

| Complexity | Model | Basic idea | What it produces |
|---|---|---|---|
| Low | Voter model | I copy a random neighbor's opinion. | Consensus or fluctuations. |
| Low-Medium | Ising model | Binary opinion + social influence + noise. | Phases, polarization, abrupt change. |
| Medium | Bounded confidence (Deffuant, Hegselmann-Krause) | Only those who think similarly to me influence me. | Separate groups, extremism, echo chambers. |
| Medium-High | Social impact theory | Impact = strength × immediacy × number of sources. | Persistence of minorities, thresholds. |
| High | Axelrod's cultural diffusion | Exchange of cultural traits. | Diversity or homogenization. |
| Very high | Agent-based models (ABM) | Agents with rules, networks, media, campaigns, emotions. | Complex scenarios, not exact prediction. |
| Very high | Machine Learning + causal inference | Estimate influence with real data. | Correlations, probable effects. |

