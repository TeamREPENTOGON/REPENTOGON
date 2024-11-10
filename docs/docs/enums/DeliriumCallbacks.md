# Enum "DeliriumCallbacks"

This is a list of callbacks exclusive to the behavior of Delirium. These callbacks live under a different namespace as they are too specific when compared to the more general ones found in [ModCallbacks](ModCallbacks.md).

### TRANSFORMATION

This callback is fired immediately before the game transforms Delirium into a boss (this also includes when Delirium returns to its normal form).
Accepts four parameters: `Delirium`, `Type`, `Variant` and `Force`.
`Delirium` is the instance of Delirium that is transforming.
`Type` and `Variant` are the type and variant of the selected boss. 
`Force` is set to `false` if the game performed the transformation by itself, `true` if the transformation was requested by the modding API.

You may return the following values:

* `true`: lets the transformation happen without changing anything.
* `false`: prevents the transformation, Delirium will remain the same.
* a table containing at least one value. The first value in this table (index 1, mandatory) is the type of the desired transformation; the second value in this table (index 2, optional, defaults to 0) is the variant of the desired transformation; other indices are ignored.
* any other value will let the transformation occur naturally.
    
|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|
|TRANSFORMATION {: .copyable } | ([EntityDelirium](../EntityDelirium.md) Delirium, int Type, int Variant, boolean Force) | - | boolean or table |

### POST_TRANSFORMATION

This callback is fired immediately after the game transform Delirium into a boss (this also includes when Delirium returns to its normal form or transforms into the same boss).
Accepts one parameter: `Delirium`, which is the instance of Delirium that transformed.
This callback returns nothing.

|Name|Function Args|Optional Args|Return Type|
|:--|:--|:--|:--|
|POST_TRANSFORMATION {: .copyable } | ([EntityDelirium](../EntityDelirium.md) Delirium) | - | void |