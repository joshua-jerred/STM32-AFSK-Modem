/**
 * @file waveform.hpp
 * @author Joshua Jerred (https://joshuajer.red)
 * @brief
 * @date 2023-08-20
 * @copyright Copyright (c) 2023
 */

#ifndef WAVEFORM_HPP_
#define WAVEFORM_HPP_

#include <etl/array.h>

static etl::array<uint16_t, 1000> SineWaveForm = {704, 708, 711, 715, 719, 723, 727, 731, 735, 738, 742, 746, 750, 754, 758, 762, 765, 769, 773, 777, 781, 785, 789, 792, 796, 800, 804, 807, 811, 815, 819, 823, 827, 830, 834, 838, 842, 845, 849, 853, 857, 860, 864, 868, 872, 875, 879, 883, 886, 890, 894, 897, 901, 905, 908, 912, 915, 919, 923, 926, 930, 933, 937, 941, 944, 948, 951, 955, 958, 962, 965, 969, 972, 976, 979, 983, 986, 990, 993, 996, 1000, 1003, 1007, 1010, 1013, 1017, 1020, 1023, 1026, 1030, 1033, 1036, 1040, 1043, 1046, 1049, 1052, 1056, 1059, 1062, 1065, 1068, 1071, 1074, 1077, 1080, 1083, 1086, 1089, 1092, 1095, 1098, 1101, 1104, 1107, 1110, 1113, 1116, 1119, 1122, 1124, 1127, 1130, 1133, 1136, 1138, 1141, 1144, 1146, 1149, 1152, 1154, 1157, 1160, 1162, 1165, 1167, 1170, 1172, 1175, 1177, 1180, 1182, 1185, 1187, 1189, 1192, 1194, 1196, 1199, 1201, 1203, 1205, 1208, 1210, 1212, 1214, 1216, 1218, 1221, 1223, 1225, 1227, 1229, 1231, 1233, 1235, 1236, 1239, 1240, 1242, 1244, 1246, 1248, 1249, 1251, 1253, 1255, 1256, 1258, 1260, 1261, 1263, 1265, 1266, 1268, 1269, 1271, 1272, 1274, 1275, 1276, 1278, 1279, 1281, 1282, 1283, 1284, 1286, 1287, 1288, 1289, 1290, 1292, 1293, 1294, 1295, 1296, 1297, 1298, 1299, 1300, 1301, 1302, 1302, 1303, 1304, 1305, 1306, 1307, 1307, 1308, 1309, 1309, 1310, 1311, 1311, 1312, 1312, 1313, 1313, 1314, 1314, 1315, 1315, 1315, 1316, 1316, 1317, 1317, 1317, 1317, 1317, 1318, 1318, 1318, 1318, 1318, 1318, 1318, 1318, 1318, 1318, 1318, 1318, 1318, 1318, 1318, 1317, 1317, 1317, 1317, 1317, 1316, 1316, 1315, 1315, 1315, 1314, 1314, 1313, 1313, 1312, 1312, 1311, 1311, 1310, 1309, 1309, 1308, 1307, 1307, 1306, 1305, 1304, 1303, 1302, 1302, 1301, 1300, 1299, 1298, 1297, 1296, 1295, 1294, 1293, 1292, 1290, 1289, 1288, 1287, 1286, 1284, 1283, 1282, 1281, 1279, 1278, 1276, 1275, 1274, 1272, 1271, 1269, 1268, 1266, 1265, 1263, 1261, 1260, 1258, 1256, 1255, 1253, 1251, 1249, 1248, 1246, 1244, 1242, 1240, 1239, 1236, 1235, 1233, 1231, 1229, 1227, 1225, 1223, 1221, 1218, 1216, 1214, 1212, 1210, 1208, 1205, 1203, 1201, 1199, 1196, 1194, 1192, 1189, 1187, 1185, 1182, 1180, 1177, 1175, 1172, 1170, 1167, 1165, 1162, 1160, 1157, 1154, 1152, 1149, 1146, 1144, 1141, 1138, 1136, 1133, 1130, 1127, 1124, 1122, 1119, 1116, 1113, 1110, 1107, 1104, 1101, 1098, 1095, 1092, 1089, 1086, 1083, 1080, 1077, 1074, 1071, 1068, 1065, 1062, 1059, 1056, 1052, 1049, 1046, 1043, 1040, 1036, 1033, 1030, 1026, 1023, 1020, 1017, 1013, 1010, 1007, 1003, 1000, 996, 993, 990, 986, 983, 979, 976, 972, 969, 965, 962, 958, 955, 951, 948, 944, 941, 937, 933, 930, 926, 923, 919, 915, 912, 908, 905, 901, 897, 894, 890, 886, 883, 879, 875, 872, 868, 864, 860, 857, 853, 849, 845, 842, 838, 834, 830, 827, 823, 819, 815, 811, 807, 804, 800, 796, 792, 789, 785, 781, 777, 773, 769, 765, 762, 758, 754, 750, 746, 742, 738, 735, 731, 727, 723, 719, 715, 711, 708, 704, 700, 696, 692, 688, 684, 681, 677, 673, 669, 665, 661, 657, 654, 650, 646, 642, 638, 634, 631, 627, 623, 619, 615, 612, 608, 604, 600, 596, 592, 589, 585, 581, 577, 573, 570, 566, 562, 558, 555, 551, 547, 543, 540, 536, 532, 529, 525, 521, 518, 514, 510, 507, 503, 499, 496, 492, 489, 485, 481, 478, 474, 471, 467, 463, 460, 456, 453, 449, 446, 442, 439, 435, 432, 428, 425, 421, 418, 414, 411, 408, 405, 401, 398, 394, 391, 388, 384, 381, 378, 375, 371, 368, 365, 362, 358, 355, 352, 349, 346, 343, 339, 336, 333, 330, 327, 324, 321, 318, 315, 312, 309, 306, 303, 300, 297, 295, 292, 289, 286, 283, 280, 278, 275, 272, 269, 267, 264, 261, 258, 256, 253, 251, 248, 246, 243, 240, 238, 235, 233, 230, 228, 225, 223, 221, 218, 216, 214, 211, 209, 207, 204, 202, 200, 198, 196, 193, 191, 189, 187, 185, 183, 181, 179, 177, 175, 173, 171, 169, 167, 165, 164, 162, 160, 158, 156, 155, 153, 151, 150, 148, 146, 145, 143, 141, 140, 138, 137, 135, 134, 132, 131, 130, 128, 127, 126, 124, 123, 122, 121, 120, 118, 117, 116, 115, 114, 113, 112, 111, 110, 109, 108, 107, 106, 105, 104, 103, 102, 102, 101, 100, 99, 99, 98, 98, 97, 96, 96, 95, 95, 94, 94, 93, 93, 93, 92, 92, 91, 91, 91, 91, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 91, 91, 91, 91, 92, 92, 93, 93, 93, 94, 94, 95, 95, 96, 96, 97, 98, 98, 99, 99, 100, 101, 102, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 120, 121, 122, 123, 124, 126, 127, 128, 130, 131, 132, 134, 135, 137, 138, 140, 141, 143, 145, 146, 148, 150, 151, 153, 155, 156, 158, 160, 162, 164, 165, 167, 169, 171, 173, 175, 177, 179, 181, 183, 185, 187, 189, 191, 193, 196, 198, 200, 202, 204, 207, 209, 211, 214, 216, 218, 221, 223, 225, 228, 230, 233, 235, 238, 240, 243, 246, 248, 251, 253, 256, 258, 261, 264, 267, 269, 272, 275, 278, 280, 283, 286, 289, 292, 295, 297, 300, 303, 306, 309, 312, 315, 318, 321, 324, 327, 330, 333, 336, 339, 343, 346, 349, 352, 355, 358, 362, 365, 368, 371, 375, 378, 381, 384, 388, 391, 394, 398, 401, 405, 408, 411, 414, 418, 421, 425, 428, 432, 435, 439, 442, 446, 449, 453, 456, 460, 463, 467, 471, 474, 478, 481, 485, 489, 492, 496, 499, 503, 507, 510, 514, 518, 521, 525, 529, 532, 536, 540, 543, 547, 551, 555, 558, 562, 566, 570, 573, 577, 581, 585, 589, 592, 596, 600, 604, 608, 612, 615, 619, 623, 627, 631, 634, 638, 642, 646, 650, 654, 657, 661, 665, 669, 673, 677, 681, 684, 688, 692, 696, 700};

#endif /* WAVEFORM_HPP_ */