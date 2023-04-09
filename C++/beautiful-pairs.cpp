// Time:  O(nlogn)
// Space: O(n)

// divide and conquer, merge sort, variant of closest pair
// reference: https://www.baeldung.com/cs/minimal-manhattan-distance
class Solution {
public:
    vector<int> beautifulPair(vector<int>& nums1, vector<int>& nums2) {
        static const int INF = numeric_limits<int>::max();
        static const int MAX_NEIGHBOR_COUNT = 8 - 1;

        vector<vector<int>> points;
        for (int i = 0; i < size(nums1); ++i) {
            points.push_back({nums1[i], nums2[i], i});
        }

        vector<int> order(size(points));
        iota(begin(order), end(order), 0);
        sort(begin(order), end(order), [&](const auto& a, const auto& b) {
            return points[a] < points[b];
        });

        vector<int> result(3, INF);
        for (int i = 0; i + 1 < size(points); ++i) {
            if (points[order[i]][0] == points[order[i + 1]][0] &&
                points[order[i]][1] == points[order[i + 1]][1]) {
                result = min(result, vector<int>{0, points[order[i]][2], points[order[i + 1]][2]});
            }
        }   
             
        const auto& manhattan_distance = [](const auto& a, const auto& b) {
            return abs(a[0] - b[0]) + abs(a[1] - b[1]);
        };

        const function<void (int, int)> merge_sort = [&](int left, int right) {
            if (left == right) {
                return;
            }
            const int mid = left + (right - left) / 2;
            const int x = points[order[mid]][0];  // added
            merge_sort(left, mid);
            merge_sort(mid + 1, right);
            vector<int> tmp;
            for (int l = left, r = mid + 1; l <= mid; ++l) {
                while (r <= right && points[order[r]][1] < points[order[l]][1]) {  // modified
                    tmp.emplace_back(order[r++]);
                }
                tmp.emplace_back(order[l]);
            }
            copy(cbegin(tmp), cend(tmp), begin(order) + left);

            // added below
            vector<int> stripe;
            for (int i = left; i <= right; ++i) {
                if (abs(points[order[i]][0] - x) <= result[0]) {
                    stripe.emplace_back(order[i]);
                }
            }
            for (int i = 0; i + 1 < size(stripe); ++i) {
                for (int j = i + 1; j < size(stripe); ++j) {
                    int x = stripe[i], y = stripe[j];
                    if (points[y][1] - points[x][1] > result[0]) {
                        break;
                    }
                    assert(j - (i + 1) + 1 <= MAX_NEIGHBOR_COUNT);
                    if (x > y) {
                        swap(x, y);
                    }
                    result = min(result, vector<int>{manhattan_distance(points[x], points[y]), x, y});
                }
            }      
        };

        if (result[0]) {
            merge_sort(0, size(points) - 1);
        }
        return {result[1], result[2]};
    }
};
