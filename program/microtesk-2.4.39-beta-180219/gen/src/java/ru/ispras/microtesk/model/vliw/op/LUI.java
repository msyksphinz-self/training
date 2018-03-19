/*
 * Copyright (c) Institute for System Programming of the Russian Academy of Sciences
 * All Rights Reserved
 *
 * Institute for System Programming of the Russian Academy of Sciences (ISP RAS)
 * 25 Alexander Solzhenitsyn st., Moscow, 109004, Russia
 * http://www.ispras.ru
 *
 * The present file was automatically generated on the basis of formal specifications.
 * It is distributed under the same terms and conditions as the derived specifications.
 *
 * N.B. PLEASE DO NOT MODIFY THIS FILE.
 */

package ru.ispras.microtesk.model.vliw.op;

import java.util.Map;
import java.math.BigInteger;
import ru.ispras.microtesk.model.Execution;
import ru.ispras.microtesk.model.ProcessingElement;
import ru.ispras.microtesk.model.data.*;
import ru.ispras.microtesk.model.memory.*;
import ru.ispras.microtesk.model.*;
import ru.ispras.microtesk.model.vliw.PE;
import ru.ispras.microtesk.model.vliw.TempVars;
import ru.ispras.microtesk.model.vliw.mode.*;
import ru.ispras.microtesk.model.memory.Location;

import static ru.ispras.microtesk.model.vliw.TypeDefs.*;

public final class LUI extends IsaPrimitive {
  private static final class Info extends IsaPrimitiveInfoAnd {
    Info() {
      super(
          IsaPrimitiveKind.OP,
          "LUI",
          LUI.class,
          null
          );
      addArgument("rt", R.INFO);
      addArgument("offset", SHORT);}

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final R rt = (R) getArgument("rt", args);
      final Immediate offset = (Immediate) getArgument("offset", args);

      return new LUI(rt, offset);
    }
  }
  public static final IsaPrimitiveInfoAnd INFO = new Info();

  public final R rt;
  public final Immediate offset;

  public LUI(final R rt, final Immediate offset) {
    assert R.INFO.isSupported(rt);
    assert SHORT.equals(offset.getType());

    this.rt = rt;
    this.offset = offset;

    addArgument("rt", rt);
    addArgument("offset", offset);
  }

  @Override
  public String syntax(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return String.format("LUI %s, 0x%x", rt.text(vars__), offset.access().load().bigIntegerValue(false));
  }

  @Override
  public String image(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return String.format("00111100000%s%s", rt.image(vars__), offset.access().load().toString());
  }

  @Override
  public void action(final ProcessingElement procElem, final TemporaryVariables tempVars) {
    final PE pe__ = (PE) procElem;
    final TempVars vars__ = (TempVars) tempVars;
    rt.access(pe__, vars__).store(Data.signExtend(WORD, offset.access().load()).shiftLeft(Data.valueOf(WORD, 0x10)));
  }
}