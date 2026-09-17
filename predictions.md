# Part II Predictions

ทำนายไว้ก่อนวัดจริง โดยดูจาก **จำนวน invocation** ล้วน ๆ (ไม่ได้เดาสุ่ม):

| Scene | Vertex placement | Fragment placement |
|---|---|---|
| 1 triangle, small (0.04x, กลางจอ) | ต่ำมาก แบนตลอด — มีแค่ 3 vertex invocation ไม่ว่า loop จะกี่รอบ | ต่ำมาก แบนตลอด — พื้นที่ปกคลุมเล็กมาก (สเกล 0.04 ยกกำลังสอง) fragment invocation น้อย |
| 1 triangle, fullscreen (4x) | ต่ำ แบนตลอด — ยังมีแค่ 3 vertex invocation เท่าเดิม ไม่ว่าจะ scale เท่าไหร่ | สูงขึ้นชัดเจนตาม loop เพราะพื้นที่ปกคลุมเกือบเต็มจอ fragment invocation เยอะมาก |
| 100k instances, small (0.04x) | สูงที่สุด พุ่งแรงตาม loop เพราะ vertex invocation = 100,000 x 3 = 300,000 | คาดว่าปานกลาง เพราะแต่ละสามเหลี่ยมเล็ก (0.04x) รวมพื้นที่ทั้งหมดไม่น่าจะเกิน fullscreen 1 รูป |

**คาดไว้ก่อนวัด:** ตัวเลขที่ใหญ่ที่สุดในตารางคือ **100k instances ในฝั่ง vertex** (300,000 invocation คูณ loop cost) และตัวที่สองคือ **fullscreen ในฝั่ง fragment** ส่วน 100k instances ในฝั่ง fragment น่าจะแพ้ fullscreen-fragment เพราะสามเหลี่ยมเล็กกว่ามาก

(ดูว่าทายถูกไหมใน write-up 2 — สปอยล์: ทายผิดข้อหนึ่ง เพราะลืมเรื่อง overdraw ของกริด 317 คอลัมน์)
