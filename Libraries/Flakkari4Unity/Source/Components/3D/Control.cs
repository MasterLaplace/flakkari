using System;
using UnityEngine;

namespace Flakkari4Unity.ECS.Components._3D
{
    public class Control : MonoBehaviour
    {
        public bool move_up = false;
        public bool move_down = false;
        public bool move_left = false;
        public bool move_right = false;
        public bool move_front = false;
        public bool move_back = false;
        public bool look_up = false;
        public bool look_down = false;
        public bool look_left = false;
        public bool look_right = false;
        public bool shoot = false;

        internal void Deserialize(byte[] data, ref int i)
        {
            int bitIndex = 0;

            move_up = (data[i] & (1 << (bitIndex % 8))) != 0;
            bitIndex++;
            move_down = (data[i] & (1 << (bitIndex % 8))) != 0;
            bitIndex++;
            move_left = (data[i] & (1 << (bitIndex % 8))) != 0;
            bitIndex++;
            move_right = (data[i] & (1 << (bitIndex % 8))) != 0;
            bitIndex++;
            move_front = (data[i] & (1 << (bitIndex % 8))) != 0;
            bitIndex++;
            move_back = (data[i] & (1 << (bitIndex % 8))) != 0;
            bitIndex++;
            look_up = (data[i] & (1 << (bitIndex % 8))) != 0;
            bitIndex++;
            look_down = (data[i] & (1 << (bitIndex % 8))) != 0;
            bitIndex++;
            i += sizeof(byte);
            look_left = (data[i] & (1 << (bitIndex % 8))) != 0;
            bitIndex++;
            look_right = (data[i] & (1 << (bitIndex % 8))) != 0;
            bitIndex++;
            shoot = (data[i] & (1 << (bitIndex % 8))) != 0;

            i += sizeof(byte);
        }
    }
}
