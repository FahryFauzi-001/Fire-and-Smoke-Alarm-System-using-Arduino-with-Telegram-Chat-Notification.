# Fire Smoke Alarm Arduino

Proyek sistem alarm kebakaran dan asap menggunakan Arduino UNO R4 WiFi.

## Struktur proyek

```text
fire-smoke-alarm-arduino/
├── src/
│   └── fire_smoke_alarm/
│       └── fire_smoke_alarm.ino
├── assets/
│   ├── wiring-diagram.png
│   └── system-architecture.png
├── .gitignore
└── README.md
```

## Diagram

Tambahkan gambar wiring yang sudah dirapikan ke `assets/wiring-diagram.png`:

![Diagram wiring](assets/wiring-diagram.png)

Tambahkan gambar arsitektur sistem yang sudah dirapikan ke
`assets/system-architecture.png`:

![Arsitektur sistem](assets/system-architecture.png)

## Kode Arduino

Kode utama untuk Arduino UNO R4 WiFi berada di
`src/fire_smoke_alarm/fire_smoke_alarm.ino`.

## Kredensial

Jangan simpan kredensial atau rahasia di repository. File `.env`, `secrets.h`,
`credentials.h`, dan `wifi_config.h` diabaikan oleh Git.
