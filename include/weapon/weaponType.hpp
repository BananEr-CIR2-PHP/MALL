#ifndef WEAPONTYPE_HPP
#define WEAPONTYPE_HPP

namespace WeaponType {
    enum WeaponType {
        None,
        Gun,
        RocketLauncher
    };

    namespace GunType {
        enum GunType {
            None,
            DesertEagle
        };
    }

    namespace RocketLauncherType {
        enum RocketLauncherType {
            None,
            Bazooka
        };
    }
}

#endif   // WEAPONTYPE_HPP