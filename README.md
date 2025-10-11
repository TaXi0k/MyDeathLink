![MyDeathLink](https://raw.githubusercontent.com/TaXi0k/MyDeathLink/refs/heads/main/.github/assets/mydeathlink-repo-cover.png)

## MyDeathLink

Welcome to repo of **MyDeathLink**. Here you'll find its source code, compiled downloads and more. If you don't care about code scroll down to [***Instalation and usage***](https://github.com/TaXi0k/MyDeathLink/blob/main/README.md#instalation-and-usage).

## Requirements

**THIS ONE IS IMPORTANT, SO BETTER READ IT!**

This project is built with **Windows only** in mind! It **DOESN'T** and probably **WON'T EVER** work on Linux or MacOS.

If you are on Windows 11 or Windows 10, everything should work.

## What MyDeathLink is?

**TL;DR:**  

MyDeathLink is a way to sync two games on two PCs so when you die, your friend also dies.

**Longer version with a bit of story:**  

This project started because I wanted to play [Archipelago](https://archipelago.gg) with my friend, but it turned out that Archipelago has very few interesting games. I thought, "Hey, I know a bit of programming and I'd love to learn more!" so I started this project. It contains the **MyDeathLink app**, which handles communication between your PCs, and **game mods** which kill you and check if you died. All the networking works by sending and listening for UDP datagrams. It uses three ports: 2137, 2138, 2139 `(R.I.P. John Paul II)`. You can find out what each port does in the [documentation](https://github.com/TaXi0k/MyDeathLink/blob/main/README.md#documentation). Communication between the app on your PC and the game on your PC works by sending segments to `127.0.0.1` (loopback address) and listening for them (**see:** communication scheme in [documentation](https://github.com/TaXi0k/MyDeathLink/blob/main/README.md#documentation)).

## Development

Below you can see development progress of the project.

| **Module**             | **Status** | **Version** |
|------------------------|------------|-------------|
| App                    |      ✅     |    0.1.0    |
| Grand Theft Auto V mod |      ✅     |    0.1.0    |
| Dark Souls 3 mod       |      🛠️     |      --     |

**Legend:**

* 📝 — planned to develop
* 🛠️ — in development
* ✅ — already developed

>
>**How can you help the project?**
>
>If you know how I could stop the app from being flagged as malware by most antivirus software, I’d really appreciate your help!
>
>If you have any other tips or want to help in any other way, feel free to message me on [Discord](https://discord.com/users/748861794637971547).

## Installation and usage

**Step 1:** Download

Head to [binaries](https://github.com/TaXi0k/MyDeathLink/tree/main/GTAV). Now download the app (MyDeathLink.exe) and mods for games you want to sync (f.e. MyDeathLink_GTAV.asi for GTA V)

> **WARNING!**
>
> This mod is **not** malware, but most antivirus software might flag it because it listens for UDP datagrams.
>
> If you don’t trust me, you can check the source code in the [APP](https://github.com/TaXi0k/MyDeathLink/tree/main/APP) folder and compile it yourself.
>
> You might even have trouble downloading the compiled .exe, but it’s possible — after an hour and some help from a friend, I finally managed to get it XD

**Step 2:** Instalation

* The app is just a single executable, so it should work by simply opening the file.
* Mod installation depends on the game you want to use. I’ll include a tutorial for GTA V only. For other games, you’ll need to google it — I’m sure you’ll figure it out.
  * First download [ScriptHookV](https://ntscorp.ru/dev-c/ScriptHookV_3586.0_889.22.zip).
  * Extract the files from the downloaded `.zip` into your game’s root folder (the one with `GTAV.exe`).
  * Finally, put the downloaded `MyDeathLink_GTAV.asi` file into the same folder.
  * When you launch the game and enter story mode, after a few seconds you should see a green notification saying the mod is enabled.

**Step 3:** Usage  

After launching the app, it will prompt you to enter your friend's IPv4 address. Since the app works only over LAN, you have to enter your friend's **local IPv4**.  

> You might ask: "Hey, but my friend doesn't live with me, so he's on a remote network."  

*Exactly!* If your friend isn't on the same network, you need to use an app like [LogMeIn Hamachi](https://vpn.net) or [Tailscale](https://tailscale.com), or anything similar.  
Now, in the **MyDeathLink** app, enter the IP address shown by the VPN app you downloaded.  

If there's anything else you'd like to know or you have any problems with the app or mod, please contact me on [Discord](https://discord.com/users/748861794637971547).

## Documentation

Here you can find all the [documentation](https://docs.google.com/document/d/1uRva6uGbwwrGnCV6bS0UMRf_SlwDhQFOZHvlztmjolk/edit?usp=sharing) of this project.

I will update it whenever anything it describes changes, so it **should** always be relevant.
