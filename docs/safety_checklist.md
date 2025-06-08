# Safety Verification Checklist

## 1. Power & Fusing
- [ ] Li-Po battery mounted in fire-safe bag  
- [ ] 5 A fuse installed on main battery lead  
- [ ] Master switch kills all power  

## 2. Failsafe & Dead-man
- [ ] Radio failsafe wired to MCU — drive & spike disabled on loss  
- [ ] Lid-switch interrupt cuts power to drive & weapon  
- [ ] MCU brown-out detection (built-in AVR) prevents errant code  

## 3. Weapon Containment
- [ ] Spike stroke < 25 mm, no projectiles  
- [ ] Weapon solenoid tied to failsafe line  
- [ ] Spike never leaves chassis envelope  

## 4. Drive Safety
- [ ] Maximum continuous current < 3 A per motor  
- [ ] Drive motors braked within < 300 ms on failsafe  
- [ ] Omni-wheel hubs securely fastened (no pop-outs)  

## 5. Software Safety
- [ ] `failsafe.poll()` runs every loop()  
- [ ] No dynamic memory allocation in firmware  
- [ ] Closed-source Wi-Fi disabled (XIAO boards inference only)  

## 6. Documentation
- [ ] Open-source CAD & firmware repo link included  
- [ ] Build & flash instructions in README  
- [ ] This checklist printed & signed at inspection  

