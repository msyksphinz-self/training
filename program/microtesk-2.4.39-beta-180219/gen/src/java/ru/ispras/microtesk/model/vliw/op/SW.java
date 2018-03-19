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

public final class SW extends IsaPrimitive {
  private static final class Info extends IsaPrimitiveInfoAnd {
    Info() {
      super(
          IsaPrimitiveKind.OP,
          "SW",
          SW.class,
          null
          );
      addArgument("base", R.INFO);
      addArgument("rt", R.INFO);
      addArgument("offset", SHORT);}

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final R base = (R) getArgument("base", args);
      final R rt = (R) getArgument("rt", args);
      final Immediate offset = (Immediate) getArgument("offset", args);

      return new SW(base, rt, offset);
    }
  }
  public static final IsaPrimitiveInfoAnd INFO = new Info();

  public final R base;
  public final R rt;
  public final Immediate offset;

  public SW(final R base, final R rt, final Immediate offset) {
    assert R.INFO.isSupported(base);
    assert R.INFO.isSupported(rt);
    assert SHORT.equals(offset.getType());

    this.base = base;
    this.rt = rt;
    this.offset = offset;

    addArgument("base", base);
    addArgument("rt", rt);
    addArgument("offset", offset);
  }

  @Override
  public String syntax(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return String.format("SW %s, %d(%s)", rt.text(vars__), offset.access().load().bigIntegerValue(), base.text(vars__));
  }

  @Override
  public String image(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return String.format("101011%s%s%s", base.image(vars__), rt.image(vars__), offset.access().load().toString());
  }

  @Override
  public void action(final ProcessingElement procElem, final TemporaryVariables tempVars) {
    final PE pe__ = (PE) procElem;
    final TempVars vars__ = (TempVars) tempVars;
    pe__.M.access(base.access(pe__, vars__).load().add(Data.signExtend(WORD, offset.access().load())).shiftRight(Data.valueOf(WORD, 0x2))).store(rt.access(pe__, vars__).load());
  }
}